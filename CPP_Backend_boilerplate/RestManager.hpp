#pragma once
#include "stdafx.h"
#include <chrono>
#include "restbed.hpp"
#include "Assets.hpp"
#include "http_code.hpp"
#include "json/json.h"

using namespace std;
using namespace restbed;

namespace Rest {
	enum class REST_FLAGS {
		REST_CHECK_FLAGS,
		IS_ABSOLUTE,
		VERIFY_JWT_ADMIN,
		VERIFY_JWT_USER
	};

	enum class REST_METHODS {
		GET, POST, PATCH, PUT, DELETE, HEAD, MERGE, OPTIONS, TRACE, CONNECT
	};

	int parse_http_code_to_int(HTTP_CODE);
	string parse_http_code_to_string(HTTP_CODE);

	struct RestDirData {
		REST_METHODS method;
		function<void(const shared_ptr<Session>)> func;
		string location;
	};

	struct FilteredRestDirData {
		string location;
		vector<RestDirData>* data;
	};

	class REQUEST {
	public:
		REQUEST(shared_ptr<Session>);
		~REQUEST();

		string getBody(void);
		template <typename T> T getHeader(string, T);
		template <typename T> T getQuery(string, T);
		template <typename T> T getParameter(string, T);

		shared_ptr<Session> getRawSession(void);
	private:
		shared_ptr<Session> session;
	};

	class RESPONSE {
	public:
		RESPONSE(shared_ptr<Session>);
		~RESPONSE();

		void send(HTTP_CODE, string);
		void json(HTTP_CODE, Json::Value);

		shared_ptr<Session> getRawSession(void);
	private:
		shared_ptr<Session> session;
	};

	class RestDirCollector {
	public:
		static bool Initialize(void);
		static bool Shutdown(void);

		static vector<RestDirData>* dirData;
		static bool isMounted;

		RestDirCollector(string = "/");
		~RestDirCollector(void);

		bool Append(REST_METHODS, const function<void(REQUEST, RESPONSE)>&, 
			initializer_list<REST_FLAGS> = {});
		bool Append(string, REST_METHODS, const function<void(REQUEST, RESPONSE)>&, 
			initializer_list<REST_FLAGS> = {});
	private:
		string dir;
	};

	function<void(shared_ptr<Session>)> WRAP_FUNC(function<void(REQUEST, RESPONSE)>,
		initializer_list<REST_FLAGS>, REST_METHODS, string);

	string parse_method_str(REST_METHODS);

}