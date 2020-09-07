#pragma once
#include "stdafx.h"
#include <cpprest/http_listener.h>
#include <chrono>


using namespace std;
using namespace web::http;
using namespace web::http::experimental::listener;

enum REST_DIR_FLAGS {
	REST_CHECK_FLAGS,
	REST_IS_ABSOLUTE
};

struct RestDirData {
	method method;
	function<void(http_request)> func;
	string location;
};

struct FilteredRestDirData {
	string location;
	vector<RestDirData> *data;
};


inline function<void(http_request)> WRAP_FUNC(function<void(http_request)>,
	vector<REST_DIR_FLAGS>, method, string);

string parse_method_str(method);

class RestDirCollector {
public:
	static bool Initialize(void);
	static bool Shutdown(void);

	static vector<RestDirData>* dirData;
	static vector<http_listener*>* ListenerList;
	static bool isMounted;

	RestDirCollector(string);
	~RestDirCollector();

	bool Append(method, const function<void(http_request)>&, const vector<REST_DIR_FLAGS> = vector<REST_DIR_FLAGS>());
private:
	string dir;
};