#include <utility>

#include "../../RestManager.hpp"
#include "../../SocketManager.hpp"

using namespace restbed;

namespace Socket {
  
  map<string, const function<void(REQUEST, RESPONSE)>> SocketManager::socketEvents = {};
  map<string, shared_ptr<WebSocket>> SocketManager::sockets = {};

  string base64_encode(const unsigned char *input, int length) {
    BIO *bmem, *b64;
    BUF_MEM *bptr;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    (void) BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    char *buff = (char *) malloc(bptr->length);
    memcpy(buff, bptr->data, bptr->length - 1);
    buff[bptr->length - 1] = 0;

    BIO_free_all(b64);

    return buff;
  }

  multimap<string, string> build_websocket_handshake_response_headers(const shared_ptr<const Request> &request) {
    auto key = request->get_header("Sec-WebSocket-Key");
    key.append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

    Byte hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(key.data()), key.length(), hash);

    multimap<string, string> headers;
    headers.insert(make_pair("Upgrade", "websocket"));
    headers.insert(make_pair("Connection", "Upgrade"));
    headers.insert(make_pair("Sec-WebSocket-Accept", base64_encode(hash, SHA_DIGEST_LENGTH)));

    return headers;
  }

  void RESPONSE::broadcast(const string &message) {
    auto response = make_shared<WebSocketMessage>(WebSocketMessage::TEXT_FRAME, message);
    response->set_mask(0);

    for (auto socket : SocketManager::sockets) {
      auto destination = socket.second;
      destination->send(response);
    }
  }

  SocketManager::SocketManager(string event, const function<void(REQUEST, RESPONSE)> &func) {
    socketEvents.insert(pair<string, const function<void(REQUEST, RESPONSE)> &>(event, func));
  }

  void SocketManager::messageHandler(const shared_ptr<WebSocket> &source, const shared_ptr<WebSocketMessage> &message) {
    const auto opcode = message->get_opcode();

    if (opcode == WebSocketMessage::PING_FRAME) {
      auto response = make_shared<WebSocketMessage>(WebSocketMessage::PONG_FRAME, message->get_data());
      source->send(response);
    }

    else if (opcode == WebSocketMessage::PONG_FRAME) {
      // PONG
      return;
    }

    else if (opcode == WebSocketMessage::CONNECTION_CLOSE_FRAME) {
      source->close();
    }

//    else if (opcode == WebSocketMessage::BINARY_FRAME) {
//      // No support of binary frame
//      CLogger::Debug("BINARY FRAME");
////      auto response = make_shared<WebSocketMessage>(WebSocketMessage::CONNECTION_CLOSE_FRAME, Bytes({10, 03}));
//      source->send("BINARY FRAME CURRENTLY UNAVAILABLE");
//    }

    else if (opcode == WebSocketMessage::TEXT_FRAME || opcode == WebSocketMessage::BINARY_FRAME) {
      chrono::steady_clock::time_point startTime = chrono::high_resolution_clock::now();
      REQUEST req(source, message);
      RESPONSE res(source, message);
      
      auto response = make_shared<WebSocketMessage>(*message);
      response->set_mask(0);

      Json::Value value;
      
      try {
        value = Assets::Parser::parseStringToJson(
            String::format("%.*s", message->get_data().size(), message->get_data().data()));

      } catch (exception &e) {
        CLogger::Debug(e.what());
        source->send(Assets::Parser::parseJsonToString(
            ServiceError::ErrorToJson(
                ServiceError::Error::DATA_PARAMETER_INVALID)));
        chrono::steady_clock::time_point stopTime = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsedTime = stopTime - startTime;
        CLogger::Debug("SOCK EVENT : %s - %.6fms",
                       req.getJson()["event"].asString().c_str(),
                       elapsedTime.count() * 1000);
        return;
      }

      const auto func = SocketManager::socketEvents[value["event"].asString()];
      
      if (func == nullptr) {
        source->send(
            Assets::Parser::parseJsonToString(
                ServiceError::ErrorToJson(ServiceError::Error::EVENT_NOT_FOUND)));
        chrono::steady_clock::time_point stopTime = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsedTime = stopTime - startTime;
        CLogger::Debug("SOCK EVENT : %s - %.6fms",
                       req.getJson()["event"].asString().c_str(),
                       elapsedTime.count() * 1000);
        return;
      }
      
      [&]() {
        
        if(!GlobalPreferences::DEBUG) {
          try{
            func(req, res);
          } catch(exception&) {
            res.emit_json(ServiceError::ErrorToJson(ServiceError::Error::INTERNAL_SERVER_ERROR));
          } catch(ServiceError::Error& e) {
            res.emit_json(ServiceError::ErrorToJson(e));
          }
        }
        
        
        try{
          func(req, res);
        } catch(std::exception& e) {
          CLogger::Error("SOCK ERR : %s ( INTERNAL_SERVER_ERROR )", e.what());
          res.emit_json(ServiceError::ErrorToJson(ServiceError::Error::INTERNAL_SERVER_ERROR));
        } catch(ServiceError::Error& e) {
          auto err = ServiceError::ErrorToJson(e);
          CLogger::Error("SOCK ERR : %s ( %s )", err["message"].asString().c_str(), err["code"].asString().c_str());
          res.emit_json(err);
        }
      }();
      chrono::steady_clock::time_point stopTime = chrono::high_resolution_clock::now();
      chrono::duration<double> elapsedTime = stopTime - startTime;
      CLogger::Debug("SOCK EVENT : %s - %.6fms",
                     req.getJson()["event"].asString().c_str(),
                     elapsedTime.count() * 1000);
    }
  }

  void SocketManager::closeHandler(const shared_ptr<WebSocket> &socket) {
    if (socket->is_open()) {
      auto response = make_shared<WebSocketMessage>(
          WebSocketMessage::CONNECTION_CLOSE_FRAME, Bytes({10, 00}));
      socket->send(response);
    }

    const auto key = socket->get_key();
    SocketManager::sockets.erase(key);

    CLogger::Debug("Closed socket connection - SocketId :  %s", key.data());
  }

  void SocketManager::errorHandler(const shared_ptr<WebSocket> &socket, error_code error) {
    const auto key = socket->get_key();
    const auto errorCode = String::format("%s", error.message().data());
    CLogger::Debug("WebSocket Errored '%s' for %s.", errorCode.c_str(), key.data());

    if (errorCode == "End of file") {
      closeHandler(socket);
    }
  }

  bool SocketManager::Initialize(Service &service) {
    shared_ptr<Resource> resource = make_shared<Resource>();
    resource->set_path(GlobalPreferences::WEBSOCKET_PATH);

    resource->set_method_handler(Rest::parse_method_str(Rest::REST_METHODS::GET), [=](const shared_ptr<Session>& session) {
      const auto request = session->get_request();
      const auto connection_header = request->get_header("connection", String::lowercase);

      if (connection_header.find("upgrade") != string::npos) {
        if (request->get_header("upgrade", String::lowercase) == "websocket") {

          const auto headers = build_websocket_handshake_response_headers(request);

          session->upgrade(SWITCHING_PROTOCOLS, headers, [](const shared_ptr<WebSocket>& socket) {
            if (socket->is_open()) {
              socket->set_close_handler(closeHandler);
              socket->set_error_handler(errorHandler);
              socket->set_message_handler(messageHandler);

              socket->send("CONNECTED", [](const shared_ptr<WebSocket>& socket) {
                const auto key = socket->get_key();
                SocketManager::sockets.insert(make_pair(key, socket));
                CLogger::Debug("SocketClient Connected - id : %s", key.data());
              });
            } else {
              CLogger::Debug("Websocket Negotiation Failed: Client closed connection");
            }
          });
        }
      }
    });

    service.publish(resource);

    return true;
  }

}// namespace Socket
