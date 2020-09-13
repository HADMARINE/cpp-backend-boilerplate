#include "RestManager.hpp"

namespace Rest{
	string parse_method_str(REST_METHODS method) {
		string method_string;
		if (method == REST_METHODS::GET) {
			method_string = "GET";
		}
		else if (method == REST_METHODS::POST) {
			method_string = "POST";
		}
		else if (method == REST_METHODS::CONNECT) {
			method_string = "CONNECT";
		}
		else if (method == REST_METHODS::DELETE) {
			method_string = "DELETE";
		}
		else if (method == REST_METHODS::HEAD) {
			method_string = "HEAD";
		}
		else if (method == REST_METHODS::MERGE) {
			method_string = "MERGE";
		}
		else if (method == REST_METHODS::OPTIONS) {
			method_string = "OPTIONS";
		}
		else if (method == REST_METHODS::PATCH) {
			method_string = "PATCH";
		}
		else if (method == REST_METHODS::PUT) {
			method_string = "PUT";
		}
		else if (method == REST_METHODS::TRACE) {
			method_string = "TRACE";
		}
		else {
			CLogger::Error("parse_method_str caught unknown method");
			method_string = "ERROR";
		}
		return method_string;
	}

	inline function<void(shared_ptr<Session>)>
		WRAP_FUNC(function<void(REQUEST, RESPONSE)> func,
			initializer_list<REST_DIR_FLAGS> flags, REST_METHODS method, string dir) {
		if (!ISDEBUGMODE) {
			return [=](shared_ptr<Session> session) {
				REQUEST req(session);
				RESPONSE res(session);
				return func(req, res);
			};
		}

		string method_string = parse_method_str(method);

		return [=](shared_ptr<Session> session) {
			chrono::steady_clock::time_point startTime = chrono::high_resolution_clock::now();
			REQUEST req(session);
			RESPONSE res(session);
			func(req, res);
			chrono::steady_clock::time_point stopTime = chrono::high_resolution_clock::now();
			chrono::duration<double> elapsedTime = stopTime - startTime;
			CLogger::Debug("REQUEST :  " + method_string + " " + dir + " - "
				+ to_string(elapsedTime.count() * 1000) + "ms");
		};
	}
}