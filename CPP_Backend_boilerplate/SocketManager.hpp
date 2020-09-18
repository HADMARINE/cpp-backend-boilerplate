#pragma once
#include "stdafx.h"
#include <vector>
#include "restbed.hpp"
#include <string>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>


namespace Socket{
	multimap<string, string>build_websocket_handshake_response_headers(const shared_ptr<const Request>&);
	string base64_encode(const unsigned char*, int);
	

	class SocketCollector {
	public:
		//SocketCollector();
		//~SocketCollector();
		static map<string, shared_ptr<WebSocket>> sockets;
		static bool Initialize(bool &result, Service&, shared_ptr<Session>);
	private:
		string event;
	};
}