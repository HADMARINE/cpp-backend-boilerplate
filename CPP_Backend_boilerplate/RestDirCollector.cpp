#include "stdafx.h"
#include "RestDirCollector.hpp"
#include "Parser.hpp"

vector<RestDirData>* RestDirCollector::dirData = new vector<RestDirData>;
vector<http_listener*>* RestDirCollector::ListenerList = new vector<http_listener*>;
bool RestDirCollector::isMounted = false;

RestDirCollector::RestDirCollector(string dir) {
	this->dir = dir;
}

RestDirCollector::~RestDirCollector() {
	Logger::Debug("RestDirCollector closed. dir : (" + this->dir + ")");
}

bool RestDirCollector::Mount() {

	if (RestDirCollector::isMounted) {
		Logger::Error("RestDirCollector has been mounted already!");
		return false;
	}

	Logger::Debug("Mounting Started.");

	Logger::Debug("Start Collecting endpoints of RestDirCollector...(1/3)");

	vector<string> *dirCollections = new vector<string>;

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

	Logger::Debug("Start Collecting endpoints of RestDirCollector...(1/3)...DONE!");

	Logger::Debug("Start Filtering RestDirData by location...(2/3)");

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

		filteredRestDirData->push_back(FilteredRestDirData{*currDir, currRddData});

		delete currDir;
		currDir = nullptr;
	}

	Logger::Debug("Start Filtering RestDirData by location...(2/3)...DONE!");

	Logger::Debug("Start Create listeners and load endpoints by support method...(3/3)");

	// Create Class Object by dirCollections
	for (unsigned int i = 0; i < filteredRestDirData->size(); i++) {
		FilteredRestDirData* currFRDD = new FilteredRestDirData;
		*currFRDD = filteredRestDirData->at(i);

		Logger::Debug("MOUNTING : " + currFRDD->location);

		http_listener *listener = new http_listener;
		*listener = http_listener(U("http://localhost:" +
			to_wstring(PORT) + Parser::parseStringToWstring(currFRDD->location)));
		
		RestDirCollector::ListenerList->push_back(listener);

		try
		{
			listener->open().then(
				[=]() {
					Logger::Debug("Dir " + currFRDD->location + " mount success");
				}
			).wait();
		}
		catch (const std::exception&)
		{
			Logger::Error("Failed to initialize RestDirCollector. dir : ("
				+ currFRDD->location + ")");
		}

		//Append listener to Listener list
		//ListenerList->push_back(&listener);
		
		for (unsigned int ci = 0; ci < currFRDD->data->size(); ci++) {
			listener->support(RestDirCollector::dirData->at(i).method, RestDirCollector::dirData->at(i).func);
		}

		delete currFRDD;//c
		currFRDD = nullptr;
	}
	Logger::Debug("Start Create listeners and load endpoints by support method...(3/3)...DONE!");

	RestDirCollector::isMounted = true;

	Logger::Info("Started " + (string)APPLICATION_NAME + " on PORT : " + to_string(PORT));

	delete filteredRestDirData;
	filteredRestDirData = nullptr;
	return true;
}

bool RestDirCollector::Append(method method,
	const function<void(http_request)>& func) {
	if (this->isMounted) {
		Logger::Error("RestDirCollector can not append any routes since mounted! Shutdown RestDirCollector first.");
		return false;
	}

	Logger::Debug("APPEND PROCESSED - DIR : " + this->dir);
	RestDirCollector::dirData->push_back(RestDirData{ method, func, this->dir });
	Logger::Debug("APPEND PROCESS COMPLETE");
	return true;
}

bool RestDirCollector::Initialize(void) {
	//RestDirCollector::dirData = new RestDirData;
	return true;
}

bool RestDirCollector::Shutdown(void) {
	Logger::Debug("SHUTDOWN PROCESS STARTED");
	for (http_listener* listener : *RestDirCollector::ListenerList) {
		listener->close().wait();
		delete listener;
		listener = nullptr;
	}

	delete RestDirCollector::ListenerList;
	RestDirCollector::ListenerList = nullptr;

	RestDirCollector::isMounted = false;

	Logger::Debug("SHUTDOWN PROCESS COMPLETE");
	return true;
}