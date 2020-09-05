#pragma once
#include "stdafx.h"
#include <cpprest/http_listener.h>

using namespace std;
using namespace web::http;
using namespace web::http::experimental::listener;


struct RestDirData {
	method method;
	function<void(http_request)> func;
	string location;
};

struct FilteredRestDirData {
	string location;
	vector<RestDirData> *data;
};

class RestDirCollector {
public:
	static bool Initialize(void);
	static bool Mount(void);
	static bool Shutdown(void);

	static vector<RestDirData>* dirData;
	static vector<http_listener*>* ListenerList;
	static bool isMounted;

	RestDirCollector(string);
	~RestDirCollector();

	bool Append(method, const function<void(http_request)>&);
private:
	string dir;
};