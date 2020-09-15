#include "stdafx.h"
#include "RestManager.hpp"
#include "SocketManager.hpp"
#include "Assets.hpp"
#include <thread>

using namespace std;

bool isRestDirCollectorStarted = false, isSocketCollectorStarted = false;

int initializeRestDirCollector() {
	if (!Rest::RestDirCollector::Initialize(isRestDirCollectorStarted)) {
		CLogger::Error("Failed to initialize RestDirCollector", true);
		return -1;
	}
}

int initializeSocketCollector() {
	if (!Socket::SocketCollector::Initialize(isSocketCollectorStarted)) {
		CLogger::Error("Failed to initialize SocketCollector");
		return -1;
	}
}


int main(void) {
	CLogger::Info("Loading Backend API ... (" + (string)APPLICATION_NAME + ")");

	thread RestDirCollectorThread(initializeRestDirCollector);
	thread SocketCollectorThread(initializeSocketCollector);

	while (!isRestDirCollectorStarted || !isSocketCollectorStarted);

	CLogger::Info("Started " + (string)APPLICATION_NAME);
	CLogger::Info("PORT INFO - REST : " + to_string(PORT) + ", SOCKET : " + to_string(PORT + 1));
	
	Assets::pauseUntilKeyPressed("Press Enter to exit");

	Rest::RestDirCollector::Shutdown();
	
	RestDirCollectorThread.detach();
	SocketCollectorThread.detach();

	return 0;
}