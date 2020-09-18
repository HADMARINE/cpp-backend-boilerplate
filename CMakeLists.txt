cmake_minimum_required(VERSION 3.10)
project(cpp-backend-boilerplate VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
        main
        src/main.cpp
        src/CLogger.cpp
        src/Parser.cpp
        src/http_code.hpp
        src/global_preferences.h
        src/restbed.hpp
        src/stdafx.h
        src/targetver.h
        src/REST_functions.cpp
        src/REST_Request.cpp
        src/REST_Response.cpp
        src/RestManager.hpp
        src/RestDirCollector.cpp
        src/SocketCollector.cpp
        src/SocketManager.hpp
        src/testdir.cpp
        src/Assets.cpp
        src/Assets.hpp
)

find_library(
        LIBRESTBED
        NAMES librestbed.a
        HINTS /usr/local/Cellar/vcpkg/2020.07/libexec/installed/x64-osx/lib
        REQUIRED
)

find_library(
        LIBJSONCPP
        NAMES libjsoncpp.a
        HINTS /usr/local/Cellar/vcpkg/2020.07/libexec/installed/x64-osx/lib
        REQUIRED
)

find_library(
        LIBCRYPTO
        NAMES libcrypto.a
        HINTS /usr/local/Cellar/vcpkg/2020.07/libexec/installed/x64-osx/lib
        REQUIRED
)

find_library(
        LIBSSL
        NAMES libssl.a
        HINTS /usr/local/Cellar/vcpkg/2020.07/libexec/installed/x64-osx/lib
        REQUIRED
)

include_directories(/usr/local/var/vcpkg/installed/x64-osx/include)

target_link_libraries(main ${LIBRESTBED} ${LIBJSONCPP} ${LIBSSL} ${LIBCRYPTO})
