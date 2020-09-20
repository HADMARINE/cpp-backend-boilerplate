#include <utility>
#include "../../RestManager.hpp"

namespace Rest {
	using namespace Json;

	RESPONSE::RESPONSE(shared_ptr<Session> session) {
		this->session = std::move(session);
	}

	RESPONSE::~RESPONSE() = default;

	void RESPONSE::send(HTTP_CODE code, const string& data) {
		multimap<string, string> headers;
		headers.insert(pair<string, string>("Content-Length", to_string(strlen(data.c_str()))));
		session->close(parse_http_code_to_int(code), data, headers);
	}
  
  void RESPONSE::setHeader(string key, string value) {
  
  }

	shared_ptr<Session> RESPONSE::getRawSession() {
		return this->session;
	}
}