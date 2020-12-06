#pragma once
#include "Assets.hpp"
#include "pch.hpp"
#include "restbed.hpp"
#include "service_error.hpp"
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <string>
#include <vector>


namespace Socket{
  using namespace std;
  using namespace restbed;
  
	multimap<string, string>build_websocket_handshake_response_headers(const shared_ptr<const Request>&);
  string base64_encode(const unsigned char*, int);
  
//  enum class SocketType {
//    TEXT_FRAME,
//
//  };

  class REQUEST {
  public:
    REQUEST(const shared_ptr<WebSocket>&, const shared_ptr<WebSocketMessage>&);
    Json::Value getJson();
    std::string getString();
    std::string getId();
    
    shared_ptr<WebSocket> getRawSource() {
      return this->source;
    }
    
    shared_ptr<WebSocketMessage> getRawMessage() {
      return this->message;
    }
    
  private:
    shared_ptr<WebSocket> source;
    shared_ptr<WebSocketMessage> message;
  };
  
  class RESPONSE {
  public:
    RESPONSE(const shared_ptr<WebSocket>&, const shared_ptr<WebSocketMessage>&);
    void broadcast(const std::string&);
    void emit_str(std::string);
    void emit_json(Json::Value);
    void emit_to_str(std::string, std::string);
    void emit_to_json(std::string, Json::Value);
    
    shared_ptr<WebSocket> getRawSource() {
      return this->source;
    }
  
    shared_ptr<WebSocketMessage> getRawMessage() {
      return this->message;
    }

  private:
    shared_ptr<WebSocket> source;
    shared_ptr<WebSocketMessage> message;
  };
  
  class SocketManager {
	public:
    explicit SocketManager(string, const function<void(REQUEST, RESPONSE)>&);
		~SocketManager() = default;
		static map<string, shared_ptr<WebSocket>> sockets;
    static map<string, const function<void(REQUEST, RESPONSE)>> socketEvents;
		static bool Initialize(Service&);
    static void messageHandler(const shared_ptr<WebSocket>&, const shared_ptr<WebSocketMessage>&);
    static void closeHandler(const shared_ptr<WebSocket>&);
    static void errorHandler(const shared_ptr<WebSocket>&, error_code);
  };
}