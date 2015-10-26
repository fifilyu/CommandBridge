/*
 * ctserver.cxx
 *
 *  Created on: 2015年10月21日
 *      Author: Fifi Lyu
 */

#include "issue.h"
#include "common.h"
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <signal.h>

void usage() {
    std::cerr
        << S_PACKAGE << "(command bridge server) "
        << VERSION << " - " << S_DESC << "\n\n"
         "用法 :\n"
            "\t" << S_PACKAGE << " -c <命令>\n"
            "\t" << S_PACKAGE << " -l [IP地址[localhost]] -c <命令>\n"
            "\t" << S_PACKAGE << " -l [IP地址[localhost]] -p [端口[44357]] -c <命令>\n"
            "\t" << S_PACKAGE << " -f -l [IP地址[localhost]] -p [端口[44357]] -c <命令>\n"
            "\t" << S_PACKAGE << " -v\n\n"
            "参数 :\n"
            "\t" << S_PACKAGE << " -c <命令>\t\t触发的命令行\n"
            "\t" << S_PACKAGE << " -f <前台执行>\t强制使用非守护方式运行\n"
            "\t" << S_PACKAGE << " -l [IP地址]\t绑定的主机IP地址\n"
            "\t" << S_PACKAGE << " -p [端口]\t\t绑定的主机端口\n"
            "\t" << S_PACKAGE << " -v <显示帮助信息>\t显示帮助信息\n"
        << std::endl;
}
int main(int argc, char* argv[]) {
    int c;
    std::string p_cmd_("");
    std::string p_listen_(HOST);
    std::string p_port_(PORT);
    bool p_daemonize_ = true;

    while (-1 != (c = getopt(argc, argv,
                             "c:"
                             "f"
                             "l:"
                             "p:"
                             "v"  // help
    ))) {
        switch (c) {
            case 'c':
                p_cmd_ = std::string(optarg);
                break;
            case 'f':
                p_daemonize_ = false;
                break;
            case 'l':
                p_listen_ = std::string(optarg);
                break;
            case 'p':
                p_port_ = std::string(optarg);
                break;
            case 'v':
                usage();
                return EXIT_SUCCESS;
            default:
                usage();
                return EXIT_FAILURE;
        }
    }

    if (p_cmd_.empty()) {
        usage();
        return EXIT_FAILURE;
    }

    // 创建守护进程
    if (p_daemonize_ && !creat_daemon(S_PACKAGE))
        return EXIT_FAILURE;

    G_LogConfig.output_to = LOGOUTPUTSTREAM_FILE;
    G_LogConfig.path = LOG_FILE;

    // 如果不存在日志文件，初始化一个空文件
    if (!is_exist(LOG_FILE))
        write_file(LOG_FILE, "");

    print_log(LOG_INFO, "Start %s", S_PACKAGE.c_str());

    try {
        zmq::context_t context(1);
        zmq::socket_t socket(context, ZMQ_REP);
        socket.bind("tcp://" + p_listen_ + ":" + p_port_);

        while (true) {
            zmq::message_t request;

            const std::string arg_(zmq_recv(socket));

            // 父进程自动忽略子进程的状态，防止出现僵死的子进程
            signal (SIGCHLD, SIG_IGN);

            int pid = fork();

            if (pid < 0) {
                print_log(LOG_ERROR, "Failed to fork child process.");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) {
                // 执行子进程
                std::vector<std::string> arg_list;
                to_vector(p_cmd_, " ", arg_list);
                to_vector(arg_, " ", arg_list);

                // 为execvp的数组结尾的NULL多分配一个列表长度
                std::vector<char *> exec_arg_list(arg_list.size() + 1);

                for (std::size_t i = 0; i != arg_list.size(); ++i)
                    exec_arg_list[i] = &arg_list[i][0];

                print_log(LOG_INFO, "Running: %s %s", p_cmd_.c_str(), arg_.c_str());

                int ret = execvp(arg_list[0].c_str(), exec_arg_list.data());

                if (ret < 0) {
                    perror(p_cmd_.c_str());
                    print_log(LOG_ERROR, "Can not execute '%s %s'.", p_cmd_.c_str(), arg_.c_str());
                }
            }

            zmq_send(socket, "0");
        }
    } catch (zmq::error_t &e) {
        G_LogConfig.output_to = LOGOUTPUTSTREAM_STDERR;
        print_log(LOG_ERROR, e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
