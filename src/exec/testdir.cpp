#include "../RestManager.hpp"

using namespace Rest;


void getHelloWorld(REQUEST req, RESPONSE res) {
  Json::Value v;
  v["HELLO"] = "WORLD";
  v["TEST"] = "TALK";
  res.json(HTTP_CODE::OK, v);
}

void postHelloWorld(REQUEST req, RESPONSE res) {
  res.json(HTTP_CODE::OK, "HELLO WORLD");
}

void deleteHelloWorld(REQUEST req, RESPONSE res) {
  auto authToken = req.getHeader("Authorization");
  res.send(HTTP_CODE::OK, "DELETE WORLD");
}

auto *rdc = new RestManager("/testdir", [](RestManager *t) {
  t->Append(REST_METHODS::GET, getHelloWorld, {REST_FLAGS::VERIFY_JWT_USER});
  t->Append(REST_METHODS::POST, postHelloWorld, {REST_FLAGS::VERIFY_JWT_USER});
  t->Append(REST_METHODS::DELETE, deleteHelloWorld);
});

auto *rdc2 = new RestManager("", [](RestManager *t) {
  t->Append(REST_METHODS::GET, getHelloWorld);
});


