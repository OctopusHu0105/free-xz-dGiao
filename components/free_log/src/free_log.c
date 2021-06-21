#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include<stdarg.h>

#include "free_log.h"

#define MINUTE_PER_SENCONDS (60UL)
#define HOUR_PER_SENCONDS   (3600UL)
#define DAY_PER_SENCONDS    (86400UL)

#define DO_CHECK_LOG_HANDLE(x, msg, action) do {\
				if (!x) {\
					printf("%s|%d Waring: %s\n", __func__, __LINE__, msg);\
					action;\
				}\
			}while(0)

const static char *c_debug_level_str[] = {
	" ",
	" E ",
	" W ",
	" I ",
	" D ",
	" V "
};

struct free_log_s {
	FILE *f;               // 
	char *filename;
	char filename_len;
	free_log_output_mode_t output;
	free_log_level_t level;
	log_cb_t cb;
	long long int remain_max_time;
	long long int last_time;
	long long int need_remain_to_time;
};


static free_log_handle_t s_free_log_handle;

static long long int _update_system_time()
{
	time_t seconds;
  	seconds = time(NULL);
  	return (long long int)seconds;
}

static FILE *_open_file(char *filename)
{
	FILE *f = fopen(filename, "w+");
	if (!f) {
		printf(" open file failed\n");
		return NULL;
	}
	return f;
}

static int _log_write_file(char *message)
{
	s_free_log_handle->last_time = _update_system_time();
	if (s_free_log_handle->last_time > s_free_log_handle->need_remain_to_time) {
		printf("Over the write time\n");
		return -1;
	}
	size_t w_size =  fwrite(message, 1, strlen(message), s_free_log_handle->f);
	fflush(s_free_log_handle->f);
	return w_size;
}

static void _update_system_timestamp(char buffer[])
{
	time_t rawtime;
	struct tm *info; 
	time( &rawtime );
 
	info = localtime( &rawtime );
	strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", info);
}

free_log_handle_t free_log_init(free_log_config_t *cfg)
{
	DO_CHECK_LOG_HANDLE(!s_free_log_handle, "already init log", return s_free_log_handle);
	s_free_log_handle = (free_log_handle_t)malloc(sizeof(struct free_log_s));
	s_free_log_handle->output = cfg->output;
	s_free_log_handle->filename = cfg->filename;

	if (s_free_log_handle->output == FREE_LOG_OUTPUT_FILE) {
		s_free_log_handle->f = _open_file(s_free_log_handle->filename);
		if (s_free_log_handle->f == NULL) {
			return NULL;
		}
	}
	DO_CHECK_LOG_HANDLE(s_free_log_handle, "Sufficient Momory", return NULL);
	return s_free_log_handle;
}

int free_log_set_max_remain_time(char *remain_time)
{
	DO_CHECK_LOG_HANDLE(s_free_log_handle, "need init log handle first", return -1);
	char *m = remain_time;
	long long int time = atoi(remain_time);
	m+=strlen(m);
	m--;
	switch(*m) {
	case 'M':
		time *= MINUTE_PER_SENCONDS;
		break;
	case 'H':
		time *= HOUR_PER_SENCONDS;
		break;
	case 'D':
		time *= DAY_PER_SENCONDS;
		break;
	default:
		printf("This is invalied mode for remain time\n");
		return -1;
	}
	s_free_log_handle->remain_max_time = time;
	s_free_log_handle->need_remain_to_time = time + _update_system_time();
	return 1;
}

void free_log(free_log_level_t level, char *format, ...)
{
	if (FREE_LOG_SYSTEM_LEVEL_DEFAULT < s_free_log_handle->level) {
		return;
	}
	va_list list;
	va_start(list, format);
	char log_buf[1024];
	char timestamp[32];
	_update_system_timestamp(timestamp);
	strcat(timestamp, c_debug_level_str[level]);
	memcpy(log_buf, timestamp, strlen(timestamp));
	vsnprintf(log_buf+strlen(timestamp), 1024, format, list);
	va_end(list);
	if (s_free_log_handle->output == FREE_LOG_OUTPUT_FILE) {
		_log_write_file(log_buf);
	} else {
		printf("%s\n", log_buf);
	}
}


void free_log_deinit()
{
	DO_CHECK_LOG_HANDLE(s_free_log_handle, "invalied log", return);
	free(s_free_log_handle);
}