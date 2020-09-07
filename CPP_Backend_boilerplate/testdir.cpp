#include "stdafx.h"
#include "RestDirCollector.hpp"

RestDirCollector* rdc = new RestDirCollector("/testdir");

void getHelloWorld(http_request req) {
	req.reply(status_codes::OK, U("HELLO WORLD"));
} 


class TestDir {
public:
	TestDir(void) {
		rdc->Append(methods::DEL, getHelloWorld);
		rdc->Append(methods::GET, getHelloWorld);
	}
	~TestDir(void) {
		delete rdc;
		rdc = nullptr;
	}
};

TestDir *td = new TestDir;