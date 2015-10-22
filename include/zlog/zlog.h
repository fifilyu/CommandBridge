/*
 * zlog.h
 *
 *  Created on: 2014年9月30日
 *      Author: Fifi Lyu
 *        Desc: 无任何非系统库依赖，功能简单的日志系统
 */

#ifndef INCLUDE_ZPSOE_ZLOG_H_
#define INCLUDE_ZPSOE_ZLOG_H_

#include "config_compiler.h"
#include <string>

using namespace std;

namespace zpsoe {

namespace zlog {

typedef enum {
  LOGOUTPUTSTREAM_FILE,
  LOGOUTPUTSTREAM_STDOUT,
  LOGOUTPUTSTREAM_STDERR
} LogOutputStream_t;

typedef enum {
  LOG_TRACE,
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL,
  LOG_OFF
} LogLevel_t;

typedef struct {
  LogLevel_t level;
  LogOutputStream_t output_to;
  size_t max_byte;
  string path;
} LogConfig_t;

extern ZPSOE_SHARED_LIB_API LogConfig_t G_LogConfig;

#define print_log(level, ...) \
  _print_log(level, __FILE__, __LINE__, __VA_ARGS__)

ZPSOE_SHARED_LIB_API void _print_log(LogLevel_t level, const char *file, int line, const char *fmt, ...);

} /* namespace zlog */

} /* namespace zpsoe */

#endif /* INCLUDE_ZPSOE_ZLOG_H_ */
