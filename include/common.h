/*
 * common.h
 *
 *  Created on: 2015年10月21日
 *      Author: Fifi Lyu
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <zmq.hpp>
#include <string>
#include <vector>
#include <sstream>
#include "zlog/zlog.h"

using namespace zpsoe::zlog;

void to_vector(
    const std::string &s, const std::string &sep, std::vector<std::string> &l);

bool write_file(
    const std::string &name, const std::string &content, const bool &append = false);

bool is_exist(const std::string &file);

bool creat_daemon(const std::string &name, const bool to_null = true);

std::string zmq_recv(zmq::socket_t &socket);

bool zmq_send(zmq::socket_t &socket, const std::string &s);



#endif /* COMMON_H_ */
