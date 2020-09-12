#pragma once
#include "stdafx.h"
#include <vector>
#include "websocketpp/server.hpp"
#include "websocketpp/config/asio_no_tls.hpp"


typedef websocketpp::server<websocketpp::config::asio> server;

class SocketCollector {
public:
	SocketCollector();
	~SocketCollector();
	static bool Initialize(bool* result);

private:
	string event;
};