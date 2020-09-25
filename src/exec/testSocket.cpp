//
// Created by HADMARINE on 2020/09/25.
//

#include "../SocketManager.hpp"

using namespace Socket;

void getTest(REQUEST req, RESPONSE res) {
  auto data = req.getJson();
  res.emit_str("test");
}

auto sm = new SocketManager("test", getTest);