#include "stdafx.h"
#include "RestManager.hpp"
#include "Parser.hpp"

namespace Rest{
	vector<RestDirData>* RestDirCollector::dirData = new vector<RestDirData>;
	bool RestDirCollector::isMounted = false;

	RestDirCollector::RestDirCollector(string dir) {
		this->dir = dir;
	}

	RestDirCollector::~RestDirCollector() {
		CLogger::Debug("RestDirCollector closed. dir : (" + this->dir + ")");
	}

	bool RestDirCollector::Initialize(bool &status) {

		if (RestDirCollector::isMounted) {
			CLogger::Error("RestDirCollector has been mounted already!");
			return false;
		}

		CLogger::Debug("Mounting Started.");

		CLogger::Debug("Start Collecting endpoints of RestDirCollector...(1/3)");

		vector<string>* dirCollections = new vector<string>;

		// Collect all endpoints of RestDirCollector
		for (unsigned int i = 0; i < RestDirCollector::dirData->size(); i++) {
			bool isContained = false;
			for (unsigned int ci = 0; ci < dirCollections->size(); ci++) {
				if (dirCollections->at(ci) == RestDirCollector::dirData->at(i).location) {
					isContained = true;
				}
			}

			if (isContained == false) {
				dirCollections->push_back(RestDirCollector::dirData->at(i).location);
			}
		}

		CLogger::Debug("Start Collecting endpoints of RestDirCollector...(1/3)...DONE!");

		CLogger::Debug("Start Filtering RestDirData by location...(2/3)");

		// Filter RestDirData by location
		vector<FilteredRestDirData>* filteredRestDirData = new vector<FilteredRestDirData>;

		for (unsigned int i = 0; i < dirCollections->size(); i++) {
			string* currDir = new string;
			*currDir = dirCollections->at(i);

			vector<RestDirData>* currRddData = new vector<RestDirData>;

			for (unsigned int ii = 0; ii < RestDirCollector::dirData->size(); ii++) {

				RestDirData* currRdd = new RestDirData;
				*currRdd = RestDirCollector::dirData->at(ii);

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

		CLogger::Debug("Start Create listeners and load endpoints by support method...(3/3)");

		Service service;

		shared_ptr<Settings> settings = make_shared<Settings>();
		
		settings->set_port(PORT);
		settings->set_worker_limit(REST_WORKER_LIMIT);



		// Create Class Object by dirCollections
		for (unsigned int i = 0; i < filteredRestDirData->size(); i++) {
			FilteredRestDirData* currFRDD = new FilteredRestDirData;
			*currFRDD = filteredRestDirData->at(i);

			CLogger::Debug("MOUNTING : " + currFRDD->location);

			shared_ptr<Resource> resource = make_shared<Resource>();
			resource->set_path(currFRDD->location);


			for (RestDirData curRdd : *currFRDD->data) {
				resource->set_method_handler(parse_method_str(curRdd.method), curRdd.func);
			}

			service.publish(resource);

			delete currFRDD;
			currFRDD = nullptr;
		}

		CLogger::Debug("Start Create listener and load endpoints...(3/3)...DONE!");


		RestDirCollector::isMounted = true;
		status = true;

		delete filteredRestDirData;
		filteredRestDirData = nullptr;

		CLogger::Debug("RestDirCollector STARTED");

		service.start(settings);

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
		if (this->isMounted) {
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

	bool RestDirCollector::Shutdown(void) {
		CLogger::Debug("SHUTDOWN PROCESS STARTED");

		RestDirCollector::dirData->clear();
		RestDirCollector::isMounted = false;

		CLogger::Debug("SHUTDOWN PROCESS COMPLETE");
		return true;
	}
}

