#include "Assets.hpp"
#include "Parser.hpp"
#include "RestManager.hpp"
#include "SocketManager.hpp"
#include "stdafx.h"
#include <thread>

using namespace std;


Service service;
shared_ptr<Settings> settings = make_shared<Settings>();

int initializeRestDirCollector() {
  if (!Rest::RestManager::Initialize(service)) {
    CLogger::Error("Failed to initialize RestManager");
    return -1;
  }
  return 0;
}

int initializeSocketCollector() {
  if (!Socket::SocketManager::Initialize(service)) {
    CLogger::Error("Failed to initialize SocketCollector");
    return -1;
  }
  return 0;
}

void startServer() {
  service.start(settings);
}


int main() {
  CLogger::Info("Loading Backend API ... (%s)", GlobalPrefences::APPLICATION_NAME);

  settings->set_port(GlobalPrefences::PORT);
  settings->set_worker_limit(GlobalPrefences::REST_WORKER_LIMIT);

  thread RestDirCollectorThread(initializeRestDirCollector);
  thread SocketCollectorThread(initializeSocketCollector);

  RestDirCollectorThread.join();
  SocketCollectorThread.join();

  thread RootServerThread(startServer);

  CLogger::Info("Started %s at port %d", GlobalPrefences::APPLICATION_NAME, GlobalPrefences::PORT);

  Assets::pauseUntilKeyPressed("Press Enter to exit");

  Rest::RestManager::Shutdown();

  service.stop();

  RootServerThread.detach();

  return 0;
}