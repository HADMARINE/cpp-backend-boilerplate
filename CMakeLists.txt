cmake_minimum_required(VERSION 3.10)
project(cpp-backend-boilerplate VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    main
    src
    src/main.cpp
    src/core/util/CLogger.cpp
    src/core/util/Parser.cpp
    src/http_code.hpp
    src/GlobalPrefences.hpp
    src/restbed.hpp
    src/pch.hpp
    src/targetver.h
    src/core/rest/REST_functions.cpp
    src/core/rest/REST_Request.cpp
    src/core/rest/REST_Response.cpp
    src/RestManager.hpp
    src/core/rest/RestDirCollector.cpp
    src/core/socket/SocketCollector.cpp
    src/SocketManager.hpp
    src/exec/testdir.cpp
    src/core/util/Assets.cpp
    src/Assets.hpp
    src/service_error.hpp
    src/core/util/service_error.cpp
    src/core/socket/SOCKET_Request.cpp
    src/core/socket/SOCKET_Response.cpp
    src/exec/testSocket.cpp
    src/core/util/Jwt.cpp src/core/util/Encrypt.cpp src/models.hpp src/core/models/models_data.cpp)

find_library(
    LIBRESTBED
    NAMES librestbed.a
    REQUIRED
)
target_link_libraries(main PRIVATE ${LIBRESTBED})

find_package(OpenSSL REQUIRED)
target_link_libraries(main PRIVATE OpenSSL::SSL OpenSSL::Crypto)

find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(main PUBLIC ${Jsoncpp_INCLUDE_DIRS})
target_link_libraries(main PRIVATE jsoncpp_lib)

find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")
target_include_directories(main PRIVATE ${JWT_CPP_INCLUDE_DIRS})

