/*
 * common.cxx
 *
 *  Created on: 2015年10月21日
 *      Author: Fifi Lyu
 */

#include "common.h"
#include "issue.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

bool write_file(
        const std::string &name, const std::string &content, const bool &append) {
    std::ofstream outfile;

    if (append)
        outfile.open(
                name.c_str(),
                std::ofstream::binary|
                std::ofstream::ate|
                std::ofstream::app
            );
    else
        outfile.open(name.c_str(), std::ofstream::binary);

    if (outfile.is_open()) {
        outfile << content;
        return true;
    }

    return false;
}

// 返回字符串，包含整个文件内容
// 忽略操作系统关于换行符的区别，强制使用 '\n' 作为换行符
std::string read_file(const std::string &file_name) {
    std::string tmp;
    std::string file_content;
    std::ifstream infile;

    infile.open(file_name.c_str(), std::ifstream::binary);

    if (!infile.is_open()) {
        return file_content;
    }

    // 以 '\n' 作为分行标识
    while (getline(infile, tmp))
        file_content += tmp + "\n";

    infile.close();

    return file_content;
}

std::string to_str(const pid_t value) {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

bool crt_pid_file(const pid_t pid, const std::string &path) {
    return write_file(path, to_str(pid));
}

bool is_exist(const std::string &file) {
    struct stat fileInfo;
    return stat(file.c_str(), &fileInfo) == 0;
}

bool creat_daemon(const std::string &name, const bool to_null) {
    G_LogConfig.output_to = LOGOUTPUTSTREAM_STDERR;

    if (name.empty()) {
        print_log(LOG_ERROR, "The daemon name is invalid.");
        return false;
    }

    const std::string pid_file_("/var/run/" + name + ".pid");

    if (is_exist(pid_file_)) {
        print_log(LOG_ERROR, "%s exists.", pid_file_.c_str());
        return false;
    }

    if (!crt_pid_file(getpid(), pid_file_)) {
        print_log(LOG_ERROR, "Cannot create pid file '%s'.", pid_file_.c_str());
        return false;
    }

    const int noclose_(to_null ? 0 : 1);

    if (daemon(0, noclose_) != 0) {
        print_log(LOG_ERROR, "Cannot run %s in the background as system daemons.", name.c_str());
        return false;
    }

    return true;
}

std::string zmq_recv(zmq::socket_t &socket) {
    zmq::message_t msg;
    socket.recv(&msg);

    return std::string(static_cast<char*>(msg.data()), msg.size());
}

bool zmq_send(zmq::socket_t &socket, const std::string &s) {
    zmq::message_t msg(s.size());
    memcpy(msg.data(), s.data(), s.size());

    return socket.send(msg);
}

/* 按照分隔符，将字符串转换为字符串数组 */
void to_vector(
    const std::string &s, const std::string &sep, std::vector<std::string> &l) {
    std::string _s(s);

  if ( _s.empty() ) return;

  size_t found;
  std::string sub_str = "";

  found = s.find(sep);

  while (found != std::string::npos) {
    /*跳过空值，比如abcd,123456,,8909
     * * 按照逗号分隔，会有空值,found=0*/
    if (found > 0) {
      sub_str = _s.substr(0, found);
      l.push_back(sub_str);
    }

    if (found >= _s.size()) return;

    _s = _s.substr(found + sep.size());
    found = _s.find(sep);
  }

  if (_s.length() > 0) l.push_back(_s);
}
