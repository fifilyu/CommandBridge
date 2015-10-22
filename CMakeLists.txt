cmake_minimum_required(VERSION 3.0)
PROJECT(CommandBridge CXX)

SET(OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

LINK_DIRECTORIES(/home/fifilyu/workspace/libs)
INCLUDE_DIRECTORIES("include" /home/fifilyu/workspace/includes)

ADD_EXECUTABLE(cmdbr-server src/server.cxx src/common.cxx src/zlog.cxx)
TARGET_LINK_LIBRARIES(cmdbr-server zmq)

ADD_EXECUTABLE(cmdbr-cli src/client.cxx src/common.cxx src/zlog.cxx)
TARGET_LINK_LIBRARIES(cmdbr-cli zmq)
