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

	function<void(shared_ptr<Session>)>
		WRAP_FUNC(function<void(REQUEST, RESPONSE)> func,
			initializer_list<REST_FLAGS> flags, REST_METHODS method, string dir) {
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
			CLogger::Debug("REST REQUEST : %s %s - %dms",
                  method_string.c_str(), dir.c_str(), elapsedTime.count() * 1000);
		};
	}

	int parse_http_code_to_int(HTTP_CODE code) {
		return (int)code;
	}

	string parse_http_code_to_string(HTTP_CODE code) {
		switch (code) {
		case HTTP_CODE::CONTINUE:
			return "Continue";
		case HTTP_CODE::SWITCHING_PROTOCOL:
			return "Switching Protocol";
		case HTTP_CODE::PROCESSING:
			return "Processing";
		case HTTP_CODE::EARLY_HINTS:
			return "Early Hints";
		case HTTP_CODE::OK:
			return "OK";
		case HTTP_CODE::CREATED:
			return "Created";
		case HTTP_CODE::ACCEPTED:
			return "Accepted";
		case HTTP_CODE::NON_AUTHORITATIVE_INFORMATION:
			return "Non-Authoritataive Information";
		case HTTP_CODE::NO_CONTENT:
			return "No Content";
		case HTTP_CODE::RESET_CONTENT:
			return "Reset Content";
		case HTTP_CODE::PARTIAL_CONTENT:
			return "Partial Content";
		case HTTP_CODE::MULTI_STATUS:
			return "Multi Status";
		case HTTP_CODE::IM_USED:
			return "IM Used";
		case HTTP_CODE::MULTIPLE_CHOICE:
			return "Multiple Choice";
		case HTTP_CODE::MOVED_PERMANENTLY:
			return "Moved Permanently";
		case HTTP_CODE::FOUND:
			return "Found";
		case HTTP_CODE::SEE_OTHER:
			return "See Other";
		case HTTP_CODE::NOT_MODIFIED:
			return "Not Modified";
		case HTTP_CODE::USE_PROXY:
			return "Use Proxy";
		case HTTP_CODE::UNUSED:
			return "Unused";
		case HTTP_CODE::TEMPORARY_REDIRECT:
			return "Temporary Redirect";
		case HTTP_CODE::PERMANENT_REDIRECT:
			return "Permanent Redirect";
		case HTTP_CODE::BAD_REQUEST:
			return "Bad Request";
		case HTTP_CODE::UNAUTHORIZED:
			return "Unauthorized";
		case HTTP_CODE::PAYMENT_REQUIRED:
			return "Payment Required";
		case HTTP_CODE::FORBIDDEN:
			return "Forbidden";
		case HTTP_CODE::NOT_FOUND:
			return "Not Found";
		case HTTP_CODE::METHOD_NOT_ALLOWED:
			return "Method Not Allowed";
		case HTTP_CODE::NOT_ACCEPTABLE:
			return "Not Acceptable";
		case HTTP_CODE::PROXY_AUTHENTICATION_REQUIRED:
			return "Proxy Authentication Required";
		case HTTP_CODE::REQUEST_TIMEOUT:
			return "Request Timeout";
		case HTTP_CODE::CONFLICT:
			return "Conflict";
		case HTTP_CODE::GONE:
			return "Gone";
		case HTTP_CODE::LENGTH_REQUIRED:
			return "Length Required";
		case HTTP_CODE::PRECONDITION_FAILED:
			return "Precondition Failed";
		case HTTP_CODE::PAYLOAD_TOO_LARGE:
			return "Payload Too Large";
		case HTTP_CODE::URI_TOO_LONG:
			return "URI Too Long";
		case HTTP_CODE::UNSUPPORTED_MEDIA_TYPE:
			return "Unsupported Media Type";
		case HTTP_CODE::REQUEST_RANGE_NOT_SATISFIABLE:
			return "Request Range Not Satisfiable";
		case HTTP_CODE::EXPECTATION_FAILED:
			return "Expectation Failed";
		case HTTP_CODE::IM_A_TEAPOT:
			return "I'm a teapot";
		case HTTP_CODE::MISDIRECTED_REQUEST:
			return "Misredirected Request";
		case HTTP_CODE::UNPROCESSABLE_ENTITY:
			return "Unprocessable Entity";
		case HTTP_CODE::LOCKED:
			return "Locked";
		case HTTP_CODE::FAILED_DEPENDENCY:
			return "Failed Dependency";
		case HTTP_CODE::UPGRADE_REQUIRED:
			return "Upgrade Required";
		case HTTP_CODE::PRECONDITION_REQUIRED:
			return "Precondition Required";
		case HTTP_CODE::TOO_MANY_REQUESTS:
			return "Too Many Requests";
		case HTTP_CODE::REQUEST_HEADER_FIELDS_TOO_LARGE:
			return "Request Header Fields Too Large";
		case HTTP_CODE::UNAVAILABLE_FOR_LEGAL_REASONS:
			return "Unavailable For Legal Reasons";
		case HTTP_CODE::INTERNAL_SERVER_ERROR:
			return "Internal Server Error";
		case HTTP_CODE::NOT_IMPLEMENTED:
			return "Not Implemented";
		case HTTP_CODE::BAD_GATEWAY:
			return "Bad Gateway";
		case HTTP_CODE::SERVICE_UNAVAILABLE:
			return "Service Unavailable";
		case HTTP_CODE::GATEWAY_TIMEOUT:
			return "Gateway Timeout";
		case HTTP_CODE::HTTP_VERSION_NOT_SUPPORTED:
			return "HTTP Version Not Supported";
		case HTTP_CODE::VARIANT_ALSO_NEGOTIATES:
			return "Variant Also Negotiates";
		case HTTP_CODE::INSUFFICIENT_STORAGE:
			return "Insufficient Storage";
		case HTTP_CODE::LOOP_DETECTED:
			return "Loop Detected";
		case HTTP_CODE::NOT_EXTENDED:
			return "Not Extended";
		case HTTP_CODE::NETWORK_AUTHENTICATION_REQUIRED:
			return "Network Authentication Required";
		default:
			CLogger::Error("parse_http_code_to_string caught unknown HTTP_CODE");
			return "ERROR";
		}
	}
}