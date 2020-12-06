#include <utility>

#include "../../RestManager.hpp"

namespace Rest {
  REQUEST::REQUEST(shared_ptr<Session> session) {
    this->session = std::move(session);
  }

  REQUEST::~REQUEST() = default;

  string REQUEST::getBody() {
    const shared_ptr<const Request> request = this->session->get_request();
    int content_length = request->get_header("Content-Length", 0);
    char *buffer = nullptr;
    session->fetch(content_length, [buffer](const shared_ptr<Session> session, const Bytes &body) {
#if defined(_WIN32) || defined(_WIN64)
          sprintf_s(buffer, 2147483647, "%.*s\n", (int) body.size(), body.data());
#else
          snprintf(buffer, 2147483647, "%.*s\n", (int) body.size(), body.data());
#endif
    });
    return buffer;
  }
  

  Json::Value REQUEST::getJson() {
    auto request = this->session->get_request();
    return Assets::Parser::parseStringToJson(this->getBody().c_str());
  }

  shared_ptr<Session> REQUEST::getRawSession() {
    return this->session;
  }

}// namespace Rest
