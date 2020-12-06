#include <utility>

#include "../../RestManager.hpp"
#include "../../pch.hpp"

namespace Rest{
	vector<RestDirData>*RestManager::dirData = new vector<RestDirData>;
	bool RestManager::isMounted = false;

  RestManager::RestManager(string dir) {
		this->dir = std::move(dir);
	}

  RestManager::RestManager(string dir, function<void(RestManager *)> func ) {
    this->dir = std::move(dir);
    func(this);
  }

  RestManager::RestManager(function<void(RestManager *)> func) {
    this->dir = "/";
    func(this);
  }

  RestManager::~RestManager() {
		CLogger::Debug("RestManager closed. core : (%s)", this->dir.c_str());
	}

	bool RestManager::Initialize(Service &service) {

		if (RestManager::isMounted) {
			CLogger::Error("RestManager has been mounted already!");
			return false;
		}

		CLogger::Debug("Mounting Started.");

		CLogger::Debug("Start Collecting endpoints of RestManager...(1/3)");

		auto* dirCollections = new vector<string>;

		// Collect all endpoints of RestManager
		for (auto & i : *RestManager::dirData) {
			bool isContained = false;
			for (auto & dirCollection : *dirCollections) {
				if (dirCollection == i.location) {
					isContained = true;
				}
			}

			if (!isContained) {
				dirCollections->push_back(i.location);
			}
		}

		CLogger::Debug("Start Collecting endpoints of RestManager...(1/3)...DONE!");

		CLogger::Debug("Start Filtering RestDirData by location...(2/3)");

		// Filter RestDirData by location
		auto* filteredRestDirData = new vector<FilteredRestDirData>;

		for (auto & dirCollection : *dirCollections) {
			auto* currDir = new string;
			*currDir = dirCollection;

			auto* currRddData = new vector<RestDirData>;

			for (auto & ii : *RestManager::dirData) {

				auto* currRdd = new RestDirData;
				*currRdd = ii;

				if (currRdd->location == *currDir) {
					currRddData->push_back(*currRdd);
				}

				delete currRdd;
				currRdd = nullptr;

			}

			filteredRestDirData->push_back(FilteredRestDirData{ *currDir, currRddData });
			
			delete currDir;
			currDir = nullptr;
		}

		CLogger::Debug("Start Filtering RestDirData by location...(2/3)...DONE!");

		CLogger::Debug("Start Create listeners and load endpoints...(3/3)");

		// Create Class Object by dirCollections
		for (auto & i : *filteredRestDirData) {
			auto* currFRDD = new FilteredRestDirData;
			*currFRDD = i;

			CLogger::Debug("MOUNTING : %s", currFRDD->location.c_str());

			shared_ptr<Resource> resource = make_shared<Resource>();
			resource->set_path(currFRDD->location);


			for (const RestDirData& curRdd : *currFRDD->data) {
				resource->set_method_handler(parse_method_str(curRdd.method), curRdd.func);
			}
   
			service.publish(resource);

			delete currFRDD;
			currFRDD = nullptr;
		}
    
    auto requestInvalidHandler = [=](shared_ptr<Session> session) {
      auto request = session->get_request();
      RESPONSE res(session);
      CLogger::Debug("REST ERR : Page Not Found");
      res.send(HTTP_CODE::NOT_FOUND, Assets::Parser::parseJsonToString(ServiceError::ErrorToJson(ServiceError::Error::PAGE_NOT_FOUND)));
    };
    
    auto requestErrorHandler = [=](const int errNum, const exception& exc, shared_ptr<Session> session) {
      auto request = session->get_request();
      RESPONSE res(session);
      CLogger::Debug("REST ERR : Internal Server Error");
      res.send(HTTP_CODE::NOT_FOUND, Assets::Parser::parseJsonToString(ServiceError::ErrorToJson(ServiceError::Error::INTERNAL_SERVER_ERROR)));
    };
    
    service.set_not_found_handler(requestInvalidHandler);
    service.set_method_not_implemented_handler(requestInvalidHandler);
    service.set_error_handler(requestErrorHandler);

		CLogger::Debug("Start Create listener and load endpoints...(3/3)...DONE!");

    delete filteredRestDirData;
    filteredRestDirData = nullptr;

    RestManager::isMounted = true;

		CLogger::Debug("RestManager STARTED");

		return true;
	}

	bool RestManager::Append(REST_METHODS method,
		const function<void(REQUEST, RESPONSE)>& func,
		initializer_list<REST_FLAGS> flags) {
		return RestManager::Append("", method, func, flags);
	}

	bool RestManager::Append(
		string dir,
		REST_METHODS method,
		const function<void(REQUEST, RESPONSE)>& func,
		initializer_list<REST_FLAGS> flags) {
		if (Rest::RestManager::isMounted) {
			CLogger::Error("RestManager can not append any routes since mounted! Shutdown RestManager first.");
			return false;
		}

		string newDir = this->dir + dir;

		CLogger::Debug("APPEND PROCESSED - DIR : %s", newDir.c_str());
    RestManager::dirData->push_back(
			RestDirData{ method, WRAP_FUNC(func, flags, method, newDir), newDir }
		);
		CLogger::Debug("APPEND PROCESS COMPLETE");
		return true;
	}

	bool RestManager::Shutdown() {
		CLogger::Debug("SHUTDOWN PROCESS STARTED");

    RestManager::dirData->clear();
    RestManager::isMounted = false;

		CLogger::Debug("SHUTDOWN PROCESS COMPLETE");
		return true;
	}
}

