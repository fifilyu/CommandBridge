/*
 * issue.h
 *
 *  Created on: 2015年10月21日
 *      Author: Fifi Lyu
 */

#ifndef ISSUE_H_
#define ISSUE_H_

#include <string>

static const std::string VERSION("v1.0.0");

// IP地址、*、127.0.0.1 或者 0.0.0.0
static const std::string HOST("127.0.0.1");

static const std::string PORT("44357");

#ifdef WIN32
static const std::string LOG_FILE("C:\\Windows\\temp\\cmdbr-server.log");
#else
static const std::string LOG_FILE("/var/log/cmdbr-server.log");
#endif

static const std::string S_PACKAGE("ctserver");

static const std::string S_DESC("命令桥服务端");

static const std::string C_PACKAGE("cmdbr-cli");

static const std::string C_DESC("命令桥客户端");

#endif /* ISSUE_H_ */
