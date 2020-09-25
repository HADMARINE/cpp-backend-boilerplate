//
// Created by HADMARINE on 2020/09/25.
//

#include "../../SocketManager.hpp"

using namespace std;
using namespace Socket;

REQUEST::REQUEST(const shared_ptr<WebSocket> &source, const shared_ptr<WebSocketMessage> &message) {
  this->source = source;
  this->message = message;
}

Json::Value REQUEST::getJson() {
  try {
    return Parser::parseStringToJson(
        String::format(
            "%.*s",
            message->get_data().size(), message->get_data().data()));
  } catch (std::exception &) {
    CLogger::Debug("Wrong JSON Value passed over");
    const auto source = this->getRawSource();
    source->send(Parser::parseJsonToString(
        ServiceError::ErrorToJson(ServiceError::Error::DATA_PARAMETER_INVALID)));
  }
}

string REQUEST::getString() {
  return String::format(
      "%.*s",
      message->get_data().size(), message->get_data().data());
}

string REQUEST::getId() {
  return source->get_key().data();
}