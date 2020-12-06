//
// Created by HADMARINE on 2020/09/25.
//

#include "../SocketManager.hpp"

using namespace Socket;


void errorTest(REQUEST req, RESPONSE res) {
  throw ServiceError::Error::DB_ERROR;
}

auto sm1 = new SocketManager("test", [](REQUEST req, RESPONSE res) {
  auto data = req.getJson();
  res.emit_json(data);
});

auto sm2 = new SocketManager("hello", errorTest);