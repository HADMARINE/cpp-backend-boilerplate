#pragma once
#include "stdafx.h"
#include <json/json.h>
#include <sstream>

class Parser {
public:
	static wstring parseStringToWstring(const string);
  static Json::Value parseStringToJson(const string);
  static string parseJsonToString(const Json::Value);
};