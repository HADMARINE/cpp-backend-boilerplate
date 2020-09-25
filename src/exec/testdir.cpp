#include "../RestManager.hpp"

using namespace Rest;


void getHelloWorld(REQUEST req, RESPONSE res) {
  //  test();
  Json::Value v;
  v["HELLO"] = "WORLD";
  v["TEST"] = "TALK";
  res.json(HTTP_CODE::OK, v);
}

void postHelloWorld(REQUEST req, RESPONSE res) {
  res.json(HTTP_CODE::OK, "HELLO WORLD");
}

void deleteHelloWorld(REQUEST req, RESPONSE res) {
  res.send(HTTP_CODE::OK, "DELETE WORLD");
}

auto *rdc = new RestManager("/testdir", [](RestManager *t) {
  t->Append(REST_METHODS::GET, getHelloWorld);
  t->Append(REST_METHODS::POST, postHelloWorld);
  t->Append(REST_METHODS::DELETE, deleteHelloWorld);
});

auto *rdc2 = new RestManager("", [](RestManager *t) {
  t->Append(REST_METHODS::GET, getHelloWorld);
});


