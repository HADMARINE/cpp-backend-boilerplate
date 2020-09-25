cmake_minimum_required(VERSION 3.10)
project(cpp-backend-boilerplate VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    main
    src/main.cpp
    src/util/CLogger.cpp
    src/util/Parser.cpp
    src/http_code.hpp
    src/GlobalPrefences.hpp
    src/restbed.hpp
    src/stdafx.h
    src/targetver.h
    src/core/rest/REST_functions.cpp
    src/core/rest/REST_Request.cpp
    src/core/rest/REST_Response.cpp
    src/RestManager.hpp
    src/core/rest/RestDirCollector.cpp
    src/core/socket/SocketCollector.cpp
    src/SocketManager.hpp
    src/exec/testdir.cpp
    src/util/Assets.cpp
    src/Assets.hpp
    src
    src/service_error.hpp src/util/service_error.cpp src/core/socket/SOCKET_Request.cpp src/core/socket/SOCKET_Response.cpp src/exec/testSocket.cpp)

find_library(
        LIBRESTBED
        NAMES librestbed.a
        HINTS $ENV{VCPKG_INSTALLED_DIR}/lib
        REQUIRED
)

find_library(
        LIBJSONCPP
        NAMES libjsoncpp.a
        HINTS $ENV{VCPKG_INSTALLED_DIR}/lib
        REQUIRED
)

find_library(
        LIBCRYPTO
        NAMES libcrypto.a
        HINTS $ENV{VCPKG_INSTALLED_DIR}/lib
        REQUIRED
)

find_library(
        LIBSSL
        NAMES libssl.a
        HINTS $ENV{VCPKG_INSTALLED_DIR}/lib
        REQUIRED
)

#find_library(
#        LIBTERM
#        NAMES libncursesw.a
#        HINTS /usr/local/Cellar/ncurses/6.2/lib
#        REQUIRED
#)

include_directories($ENV{VCPKG_INSTALLED_DIR}/include)

target_link_libraries(main
        ${LIBRESTBED}
        ${LIBJSONCPP}
        ${LIBSSL}
        ${LIBCRYPTO}
#        ${LIBTERM}
        )
