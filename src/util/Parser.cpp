#include "../Parser.hpp"

using namespace Json;

wstring Parser::parseStringToWstring(const string str) {
  wstring *wstr = new wstring;
  wstr->assign(str.begin(), str.end());
  return *wstr;
};

Value Parser::parseStringToJson(const string str) {
  CharReaderBuilder rbuilder;
  string errs;
  Value val;
  stringstream ss;
  
  ss << str;
  
  rbuilder["collectComments"] = false;
  
  bool isSuccess = parseFromStream(rbuilder, ss, &val, &errs);
  
  if(!isSuccess) {
    CLogger::Error("%s", errs.c_str());
  }
  
  return nullptr;
}

string Parser::parseJsonToString(const Value val) {
  StreamWriterBuilder wbuilder;
  string str_data = writeString(wbuilder, val);
  return str_data;
}