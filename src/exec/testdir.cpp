#include "../RestManager.hpp"

using namespace Rest;

void getHelloWorld(REQUEST req, RESPONSE res) {
  throw "1";
  res.send(HTTP_CODE::OK, "HELLO WORLD");
}

void deleteHelloWorld(REQUEST req, RESPONSE res) {
  res.send(HTTP_CODE::OK, "DELETE WORLD");
}

auto *rdc2 = new RestDirCollector("/testdir", [](RestDirCollector* t) {
  t->Append(REST_METHODS::GET, getHelloWorld);
  t->Append(REST_METHODS::DELETE, deleteHelloWorld);
});
