#include "stdafx.h"
#include "RestManager.hpp"
#include "SocketManager.hpp"
#include "Assets.hpp"
#include <thread>

using namespace std;


Service service;
shared_ptr<Settings> settings = make_shared<Settings>();

int initializeRestDirCollector() {
	if (!Rest::RestDirCollector::Initialize(service)) {
		CLogger::Error("Failed to initialize RestDirCollector", true);
		return -1;
	}
	return 0;
}

int initializeSocketCollector() {
	if (!Socket::SocketCollector::Initialize(service)) {
        CLogger::Error("Failed to initialize SocketCollector");
        return -1;
    }
	return 0;
}

void startServer() {
    service.start(settings);
}


int main(void) {
	CLogger::Info("Loading Backend API ... (" + (string)APPLICATION_NAME + ")");

	settings->set_port(PORT);
	settings->set_worker_limit(REST_WORKER_LIMIT);

	thread RestDirCollectorThread(initializeRestDirCollector);
	thread SocketCollectorThread(initializeSocketCollector);

	RestDirCollectorThread.join();
    SocketCollectorThread.join();

    thread RootServerThread(startServer);

	CLogger::Info("Started " + (string)APPLICATION_NAME + "at port " + to_string(PORT));

	Assets::pauseUntilKeyPressed("Press Enter to exit");

	Rest::RestDirCollector::Shutdown();

	RootServerThread.detach();

	return 0;
}