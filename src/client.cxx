/*
 * ctclient.cxx
 *
 *  Created on: 2015年10月21日
 *      Author: Fifi Lyu
 */

#include "issue.h"
#include "common.h"
#include <unistd.h>
#include <iostream>

void usage() {
    std::cerr
        << C_PACKAGE << "(command bridge client) "
        << VERSION << " - " << C_DESC << "\n\n"
         "用法 :\n"
            "\t" << C_PACKAGE << " -a <参数>\n"
            "\t" << C_PACKAGE << " -h [主机名[localhost]] -a <参数>\n"
            "\t" << C_PACKAGE << " -h [主机名[localhost]] -p [主机端口[44357]] -a <参数>\n"
            "\t" << C_PACKAGE << " -v\n\n"
            "参数 :\n"
            "\t" << C_PACKAGE << " -a <参数>\t\t发送参数到主机\n"
            "\t" << C_PACKAGE << " -h [主机地址]\t\t连接主机地址\n"
            "\t" << C_PACKAGE << " -p [主机端口]\t\t连接主机端口\n"
            "\t" << C_PACKAGE << " -v <显示帮助信息>\t显示帮助信息\n"
        << std::endl;
}

int main(int argc, char* argv[]) {
    int c;
    std::string p_arg_("");
    std::string p_host_(HOST);
    std::string p_port_(PORT);

    while (-1 != (c = getopt(argc, argv,
                             "a:"
                             "h:"
                             "p:"
                             "v"  // help
    ))) {
        switch (c) {
            case 'a':
                p_arg_ = std::string(optarg);
                break;
            case 'h':
                p_host_ = std::string(optarg);
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

    if (p_arg_.empty()) {
        usage();
        return EXIT_FAILURE;
    }

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);

    socket.connect("tcp://" + p_host_ + ":" + PORT);

    zmq_send(socket, p_arg_);

    if (zmq_recv(socket) == "0")
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}
