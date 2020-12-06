//
// Created by HADMARINE on 2020/09/25.
//

#include "../../SocketManager.hpp"
#include "../../RestManager.hpp"

using namespace std;
using namespace Socket;

RESPONSE::RESPONSE(const shared_ptr<WebSocket>& source, const shared_ptr<WebSocketMessage>& message) {
  this->source = source;
  this->message = message;
}

void RESPONSE::emit_str(std::string message) {
  source->send(message);
}

void RESPONSE::emit_json(Json::Value message) {
  Json::Value root;
  
  root["result"] = true;
  root["data"] = message;
  
  source->send(Assets::Parser::parseJsonToString(message));
}

void RESPONSE::emit_to_str(string id, string message) {
  CLogger::Debug("emit_to Test needed");
  source->set_key(id);
  source->send(message);
}

void RESPONSE::emit_to_json(string id, Json::Value message) {
  CLogger::Debug("emit_to Test needed");
  source->set_key(id);
  source->send(Assets::Parser::parseJsonToString(message));
}