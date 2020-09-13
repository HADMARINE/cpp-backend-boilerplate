#include "RestManager.hpp"

namespace Rest {
	RESPONSE::RESPONSE(shared_ptr<Session> session) {
		this->session = session;
	}

	RESPONSE::~RESPONSE() {

	}

	void RESPONSE::send(HTTP_CODE code, string data) {
		const shared_ptr<const Request> request = this->session->get_request();
		multimap<string, string> headers;
		headers.insert(pair<string, string>("Content-Length", to_string(strlen(data.c_str()))));
		session->close(OK, data, headers);
	}

	void RESPONSE::json(HTTP_CODE code, multimap<string, string> data) {

	}

}