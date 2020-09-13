#include "RestManager.hpp"

namespace Rest {
	REQUEST::REQUEST(shared_ptr<Session> session) {
		this->session = session;
	}

	REQUEST::~REQUEST() {

	}

	string REQUEST::getBody(void) {
		const shared_ptr<const Request> request = this->session->get_request();
		int content_length = request->get_header("Content-Length", 0);
		char* buffer;
		session->fetch(content_length, [buffer](const shared_ptr<Session> session, const Bytes& body) {
			sprintf_s(buffer, 2147483647, "%.*s\n", (int)body.size(), body.data());
			}
		);
		return buffer;
	}

	template <typename T>
	T REQUEST::getHeader(string header, T default_value) {
		const shared_ptr<const Request> request = this->session->get_request();
		return request->get_header(header, default_value);
	}

	template <typename T>
	T REQUEST::getQuery(string query, T default_value) {
		const shared_ptr<const Request> request = this->session->get_request();
		return request->get_query_parameter(query, default_value);
	}

	template <typename T>
	T REQUEST::getParameter(string param, T default_value) {
		const shared_ptr<const Request> request = this->session->get_request();
		return request->get_path_parameter(param, default_value);
	}

	shared_ptr<Session> REQUEST::getRawSession() {
		return this->session;
	}

}
