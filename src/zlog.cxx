/*
 * zlog.cxx
 *
 *  Created on: 2014年9月30日
 *      Author: Fifi Lyu
 */

#include "zlog/config_platform.h"
#include "zlog/zlog.h"

#ifdef PLATFORM_WIN32
#   include <process.h>
#   include <Windows.h>
#else
#   include <errno.h>
#   include <unistd.h>
#endif

#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <sstream>
#include <ctime>
#include <iostream>

#ifdef PLATFORM_WIN32
static const string OS_LINE_FEED("\r\n");
#else
static const string OS_LINE_FEED("\n");
#endif

namespace zpsoe {

namespace zlog {

#ifdef PLATFORM_WIN32
// 104857600 byte == 100 MiB
LogConfig_t G_LogConfig = {LOG_INFO, LOGOUTPUTSTREAM_FILE, 104857600, "C:\\Windows\\temp\\tmp.log"};
#else
// 104857600 byte == 100 MiB
LogConfig_t G_LogConfig = {LOG_INFO, LOGOUTPUTSTREAM_FILE, 104857600, "/tmp/tmp.log" };
#endif

string int_to_str(const int &value) {
  ostringstream ss_;
  ss_ << value;
  return ss_.str();
}

string errno_to_str() {
#ifdef PLATFORM_WIN32
    string msg_(_strerror(NULL));
    const size_t size_ = msg_.size();

    if (size_) msg_.erase(size_-1);

    return msg_ + "(errno: " + int_to_str(errno) + ")";
#else
    return string(strerror(errno)) + "(errno: " + int_to_str(errno) + ")";
#endif
}

size_t get_size_in_byte(const string &file_name) {
  struct stat buf_;

  if (stat(file_name.c_str(), &buf_) == 0)
      return buf_.st_size;

  return 0;
}

// 获取当前时间,比如 2011-11-16 14:06:36
string get_current_date() {
    char buf[80];
    time_t t = time(NULL);
    strftime(buf, 80, "%Y-%m-%d %H:%M:%S", localtime(&t));
    return string(buf);
}

void open_log_file(FILE **file) {
  const size_t old_log_size_ = get_size_in_byte(G_LogConfig.path);

  if (old_log_size_ >= G_LogConfig.max_byte)
    // 覆盖
    *file = fopen(G_LogConfig.path.c_str(), "wb");
  else
    // 追加
    *file = fopen(G_LogConfig.path.c_str(), "ab+");

  if (*file == NULL) {
    const string err_("cannot open \"" + G_LogConfig.path + "\": " + errno_to_str());
    cerr << err_ << endl;
    exit(EXIT_FAILURE);
  }
}

ZPSOE_SHARED_LIB_API void _print_log(
    LogLevel_t level, const char *file, int line, const char *fmt, ...) {
  // 关闭日志
  if (level == LOG_OFF) return;

  // 关闭比用户设置更加详细的日志
  if (level < G_LogConfig.level) return;

  FILE *log_file_ = NULL;

  switch (G_LogConfig.output_to) {
    case LOGOUTPUTSTREAM_STDOUT:
      log_file_ = stdout;
      break;
    case LOGOUTPUTSTREAM_STDERR:
      log_file_ = stderr;
      break;
    case LOGOUTPUTSTREAM_FILE:
      open_log_file(&log_file_);
      break;
    default:
      log_file_ = stdout;
      break;
  }

#ifdef PLATFORM_WIN32
    static int pid_ = _getpid();
#else
    static int pid_ = getpid();
#endif

  fprintf(log_file_, "[%s] [%d] ", get_current_date().c_str(), pid_);

  if (level <= LOG_DEBUG)
    fprintf(log_file_, "%s:%d ", file, line);

  va_list ap_;
  va_start(ap_, fmt);
  vfprintf(log_file_, fmt, ap_);
  va_end(ap_);

  fprintf(log_file_, OS_LINE_FEED.c_str());
  fflush(log_file_);

  if (G_LogConfig.output_to == LOGOUTPUTSTREAM_FILE)
    fclose(log_file_);
}

} /* namespace zlog */

} /* namespace zpsoe */
