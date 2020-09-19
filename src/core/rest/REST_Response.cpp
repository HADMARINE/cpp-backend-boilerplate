#include <utility>
#include "../../RestManager.hpp"

namespace Rest {
	using namespace Json;

	RESPONSE::RESPONSE(shared_ptr<Session> session) {
		this->session = std::move(session);
	}

	RESPONSE::~RESPONSE() = default;

	void RESPONSE::send(HTTP_CODE code, const string& data) {
		const shared_ptr<const Request> request = this->session->get_request();
		multimap<string, string> headers;
		headers.insert(pair<string, string>("Content-Length", to_string(strlen(data.c_str()))));
		session->close(parse_http_code_to_int(code), data, headers);
	}

	void RESPONSE::json(HTTP_CODE code, Json::Value data) {
		const shared_ptr<const Request> request = this->session->get_request();
  
		multimap<string, string> headers;
		
		string str_data = Parser::parseJsonToString(std::move(data));

		headers.insert(pair<string, string>("Content-Length",
			to_string(strlen(str_data.c_str()))));
		session->close(parse_http_code_to_int(code), str_data, headers);
	}

	shared_ptr<Session> RESPONSE::getRawSession() {
		return this->session;
	}
}