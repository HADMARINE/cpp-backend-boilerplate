#include <utility>

#include "stdafx.h"
#include "RestManager.hpp"
#include "Parser.hpp"

namespace Rest{
	vector<RestDirData>* RestDirCollector::dirData = new vector<RestDirData>;
	bool RestDirCollector::isMounted = false;

	RestDirCollector::RestDirCollector(string dir) {
		this->dir = std::move(dir);
	}

	RestDirCollector::~RestDirCollector() {
		CLogger::Debug("RestDirCollector closed. dir : (" + this->dir + ")");
	}

	bool RestDirCollector::Initialize(Service &service) {

		if (RestDirCollector::isMounted) {
			CLogger::Error("RestDirCollector has been mounted already!");
			return false;
		}

		CLogger::Debug("Mounting Started.");

		CLogger::Debug("Start Collecting endpoints of RestDirCollector...(1/3)");

		auto* dirCollections = new vector<string>;

		// Collect all endpoints of RestDirCollector
		for (auto & i : *RestDirCollector::dirData) {
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

		CLogger::Debug("Start Collecting endpoints of RestDirCollector...(1/3)...DONE!");

		CLogger::Debug("Start Filtering RestDirData by location...(2/3)");

		// Filter RestDirData by location
		auto* filteredRestDirData = new vector<FilteredRestDirData>;

		for (auto & dirCollection : *dirCollections) {
			auto* currDir = new string;
			*currDir = dirCollection;

			auto* currRddData = new vector<RestDirData>;

			for (auto & ii : *RestDirCollector::dirData) {

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

			CLogger::Debug("MOUNTING : " + currFRDD->location);

			shared_ptr<Resource> resource = make_shared<Resource>();
			resource->set_path(currFRDD->location);


			for (const RestDirData& curRdd : *currFRDD->data) {
				resource->set_method_handler(parse_method_str(curRdd.method), curRdd.func);
			}

			service.publish(resource);

			delete currFRDD;
			currFRDD = nullptr;
		}

		CLogger::Debug("Start Create listener and load endpoints...(3/3)...DONE!");

        delete filteredRestDirData;
        filteredRestDirData = nullptr;

		RestDirCollector::isMounted = true;

		CLogger::Debug("RestDirCollector STARTED");

		return true;
	}

	bool RestDirCollector::Append(REST_METHODS method,
		const function<void(REQUEST, RESPONSE)>& func,
		initializer_list<REST_FLAGS> flags) {
		return RestDirCollector::Append("", method, func, flags);
	}

	bool RestDirCollector::Append(
		string dir,
		REST_METHODS method,
		const function<void(REQUEST, RESPONSE)>& func,
		initializer_list<REST_FLAGS> flags) {
		if (Rest::RestDirCollector::isMounted) {
			CLogger::Error("RestDirCollector can not append any routes since mounted! Shutdown RestDirCollector first.");
			return false;
		}

		string newDir = this->dir + dir;

		CLogger::Debug("APPEND PROCESSED - DIR : " + newDir);
		RestDirCollector::dirData->push_back(
			RestDirData{ method, WRAP_FUNC(func, flags, method, newDir), newDir }
		);
		CLogger::Debug("APPEND PROCESS COMPLETE");
		return true;
	}

	bool RestDirCollector::Shutdown() {
		CLogger::Debug("SHUTDOWN PROCESS STARTED");

		RestDirCollector::dirData->clear();
		RestDirCollector::isMounted = false;

		CLogger::Debug("SHUTDOWN PROCESS COMPLETE");
		return true;
	}
}

