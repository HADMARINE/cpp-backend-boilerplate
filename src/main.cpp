#include "Assets.hpp"
#include "RestManager.hpp"
#include "SocketManager.hpp"
#include "pch.hpp"
#include <thread>

using namespace std;
using namespace restbed;

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
//  Json::Value v;
//  v["TEST"] = 1;
//  auto iv = v.begin()->asInt();
//  auto ivK = v.begin().key().asString();
//  std::cout << iv << " " << ivK << std::endl;
  
  CLogger::Info("Loading Backend API ... (%s)", GlobalPreferences::APPLICATION_NAME);

  settings->set_port(GlobalPreferences::PORT);
  settings->set_worker_limit(GlobalPreferences::REST_WORKER_LIMIT);

  thread RestDirCollectorThread(initializeRestDirCollector);
  thread SocketCollectorThread(initializeSocketCollector);

  RestDirCollectorThread.join();
  SocketCollectorThread.join();

  thread RootServerThread(startServer);

  CLogger::Success("Started %s at port %d", GlobalPreferences::APPLICATION_NAME, GlobalPreferences::PORT);

  Assets::pauseUntilKeyPressed("Press Enter to exit");

  Rest::RestManager::Shutdown();

  service.stop();

  RootServerThread.detach();

  return 0;
}