#include "stdafx.h"
#include "RestManager.hpp"

using namespace Rest;

RestDirCollector* rdc = new RestDirCollector("/testdir");

void getHelloWorld(REQUEST req, RESPONSE res) {
	res.send(HTTP_CODE::OK, "HELLO WORLD");
} 


class TestDir {
public:
	TestDir(void) {
		rdc->Append(REST_METHODS::DELETE, getHelloWorld);
		rdc->Append(REST_METHODS::GET, getHelloWorld);
	}
	~TestDir(void) {
		delete rdc;
		rdc = nullptr;
	}
};

TestDir *td = new TestDir;