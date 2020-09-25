#include "../Parser.hpp"

using namespace Json;
using namespace std;

wstring Parser::parseStringToWstring(const string str) {
  wstring *wstr = new wstring;
  wstr->assign(str.begin(), str.end());
  return *wstr;
};

Value Parser::parseStringToJson(const string str) {
  const auto stringLength = static_cast<int>(str.length());
  JSONCPP_STRING err;
  Value root;
  
  CharReaderBuilder builder;
  const unique_ptr<CharReader> reader(builder.newCharReader());
  if (!reader->parse(str.c_str(), str.c_str() + stringLength, &root,
                     &err)) {
    CLogger::Debug("Failed to parse json string");
    throw runtime_error("Failed to parse json string");
  }
  return root;
}

string Parser::parseJsonToString(const Value val) {
  StreamWriterBuilder wbuilder;
  string str_data = writeString(wbuilder, val);
  return str_data;
}