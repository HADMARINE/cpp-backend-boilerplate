#include "stdafx.h"
#include "RestDirCollector.hpp"
#include "SocketCollector.hpp"
#include "Assets.hpp"
#include <thread>

using namespace std;

bool isRestDirCollectorStarted = false, isSocketCollectorStarted = false;

int initializeRestDirCollector() {
	if (!RestDirCollector::Initialize()) {
		Logger::Error("Failed to initialize RestDirCollector", true);
		return 1;
	}
	 
	Logger::Debug("RestDirCollector STARTED");

	isRestDirCollectorStarted = true;

	while (true);
}

int initializeSocketCollector() {
	if (!SocketCollector::Initialize(&isSocketCollectorStarted)) {
		Logger::Error("Failed to initialize SocketCollector");
		return -1;
	}
}



int main(void) {
	Logger::Info("Loading Backend API ... (" + (string)APPLICATION_NAME + ")");

	thread RestDirCollectorThread(initializeRestDirCollector);
	thread SocketCollectorThread(initializeSocketCollector);

	while (!isRestDirCollectorStarted || !isSocketCollectorStarted);

	Logger::Info("Started " + (string)APPLICATION_NAME);
	Logger::Info("PORT INFO - REST : " + to_string(PORT) + ", SOCKET : " + to_string(PORT + 1));
	
	Assets::pauseUntilKeyPressed("Press Enter to exit");

	RestDirCollector::Shutdown();
	
	RestDirCollectorThread.detach();
	SocketCollectorThread.detach();

	return 0;
}