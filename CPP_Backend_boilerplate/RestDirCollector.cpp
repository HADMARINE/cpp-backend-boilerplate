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

bool RestDirCollector::Initialize() {

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
					Logger::Debug("MOUNTING: " + currFRDD->location + " ...SUCCESS!");
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
		
		for (RestDirData curRdd: *currFRDD->data) {
			listener->support(curRdd.method, curRdd.func);
		}

		delete currFRDD;//c
		currFRDD = nullptr;
	}
	Logger::Debug("Start Create listeners and load endpoints by support method...(3/3)...DONE!");

	RestDirCollector::isMounted = true;

	delete filteredRestDirData;
	filteredRestDirData = nullptr;
	return true;
}

bool RestDirCollector::Append(method method,
	const function<void(http_request)>& func, const vector<REST_DIR_FLAGS> flags) {

	if (this->isMounted) {
		Logger::Error("RestDirCollector can not append any routes since mounted! Shutdown RestDirCollector first.");
		return false;
	}

	Logger::Debug("APPEND PROCESSED - DIR : " + this->dir);
	RestDirCollector::dirData->push_back(RestDirData{ method, WRAP_FUNC(func, flags, method, this->dir), this->dir });
	Logger::Debug("APPEND PROCESS COMPLETE");
	return true;
}

bool RestDirCollector::Shutdown(void) {
	Logger::Debug("SHUTDOWN PROCESS STARTED");
	for (http_listener* listener : *RestDirCollector::ListenerList) {
		listener->close().wait();
		delete listener;
		listener = nullptr;
	}

	RestDirCollector::dirData->clear();
	RestDirCollector::ListenerList->clear();
	RestDirCollector::isMounted = false;

	Logger::Debug("SHUTDOWN PROCESS COMPLETE");
	return true;
}



inline function<void(http_request)> WRAP_FUNC(function<void(http_request)> func,
	vector<REST_DIR_FLAGS> flags, method method, string dir) {
	if (!ISDEBUGMODE) {
		return func;
	}

	string method_string = parse_method_str(method);

	return [=](http_request req) {

		chrono::steady_clock::time_point startTime = chrono::high_resolution_clock::now();
		func(req);
		chrono::steady_clock::time_point stopTime = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsedTime = stopTime - startTime;
		Logger::Debug("REQUEST :  " + method_string + " " + dir + " - "
			+ to_string(elapsedTime.count() * 1000) + "ms");
	};
}

string parse_method_str(method method) {
	string method_string;
	if (method == methods::GET) {
		method_string = "GET";
	}
	else if (method == methods::POST) {
		method_string = "POST";
	}
	else if (method == methods::CONNECT) {
		method_string = "CONNECT";
	}
	else if (method == methods::DEL) {
		method_string = "DELETE";
	}
	else if (method == methods::HEAD) {
		method_string = "HEAD";
	}
	else if (method == methods::MERGE) {
		method_string = "MERGE";
	}
	else if (method == methods::OPTIONS) {
		method_string = "OPTIONS";
	}
	else if (method == methods::PATCH) {
		method_string = "PATCH";
	}
	else if (method == methods::PUT) {
		method_string = "PUT";
	}
	else if (method == methods::TRCE) {
		method_string = "TRACE";
	}
	return method_string;
}