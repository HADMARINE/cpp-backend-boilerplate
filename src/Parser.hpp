#pragma once
#include "stdafx.h"
#include <json/json.h>
#include <sstream>

class Parser {
public:
	static std::wstring parseStringToWstring(const std::string);
  static Json::Value parseStringToJson(const std::string);
  static std::string parseJsonToString(const Json::Value);
};