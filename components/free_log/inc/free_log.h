#ifndef __FREE_LOG_H__
#define __FREE_LOG_H__


#define RED       "\033[0;31m"  // error
#define YELLOW    "\033[0;33m"  // warning 
#define BLUE      "\033[0;34m"  // info 
#define WHITE     "\033[0;37m"  // debug
#define RESET     "\033[0m"     // reset color

#define LOG_E     RED
#define LOG_W     YELLOW
#define LOG_I     BLUE
#define LOG_D     WHITE

#define FREE_LOG_SYSTEM_LEVEL_DEFAULT     3
#define FREE_LOG_LEVEL(L) FREE_LOG_LEVEL##L

typedef enum {
	FREE_LOG_LEVEL(NONE) = 0,  // FREE_LOG_LEVEL_NONE
	FREE_LOG_LEVEL(ERROR),     // FREE_LOG_LEVEL_ERROR
	FREE_LOG_LEVEL(WARNING),   // FREE_LOG_LEVEL_WARNING
	FREE_LOG_LEVEL(INFO),      // FREE_LOG_LEVEL_INFO
	FREE_LOG_LEVEL(DEBUG),     // FREE_LOG_LEVEL_DEBUG
	FREE_LOG_LEVEL(VERBOSE),   // FREE_LOG_LEVEL_VERBOSE
} free_log_level_t;

typedef enum {
	FREE_LOG_OUTPUT_SCREEN,
	FREE_LOG_OUTPUT_FILE,
} free_log_output_mode_t;

typedef struct {
	 /* The max time to remain the device, for example: 10M(10 minutes),  10H(10 hours), 10D(10 days),
	 (Now just support this three ways, M, H, D),when exceed the setting time it will stop record log information*/
	char *max_remain_time;
	char *filename;

	free_log_output_mode_t output;
	free_log_level_t level;
} free_log_config_t;

typedef struct free_log_s *free_log_handle_t;

typedef void (*log_cb_t) (free_log_level_t level, char *msg, size_t size, void *ctx);

free_log_handle_t free_log_init(free_log_config_t *cfg);

int free_log_set_max_remain_time(char *remain_time);  

void free_log_register_cb(log_cb_t cb);


void free_log(free_log_level_t level, char *format, ...);

static inline void free_debug(free_log_level_t level, char *buf, size_t size)
{
	fprintf(stderr, "%.*s", buf, size);
}

#endif // __FREE_LOG_H__
