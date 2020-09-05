#include "stdafx.h"
#include "RestDirCollector.hpp"
#include "Assets.hpp"

using namespace std;

void testApi(http_request req) {
	req.reply(status_codes::OK, L"HELLO WORLD!");
}

int main(void) {
	Logger::Info("Loading CPP Backend API...");
	RestDirCollector::Initialize();
	//TestDir::main();

	RestDirCollector *testRdc = new RestDirCollector("/dd"), *test2Rdc = new RestDirCollector("/hello");
	testRdc->Append(methods::GET, testApi);
	test2Rdc->Append(methods::GET, testApi);
	RestDirCollector::Mount();
	//system("pause");
	//test2Rdc->Append(methods::GET, testApi);
	//test2Rdc->Mount();
	Assets::pauseUntilKeyPressed("Press Enter to exit");
	RestDirCollector::Shutdown();
	return 0;
}