#include "../../Assets.hpp"

using namespace std;

namespace Assets::Parser {
  wstring parseStringToWstring(const string &str) {
    wstring wstr;
    wstr.assign(str.begin(), str.end());
    return wstr;
  };

  Json::Value parseStringToJson(const string &str) {
    const auto stringLength = static_cast<int>(str.length());
    JSONCPP_STRING err;
    Json::Value root;

    Json::CharReaderBuilder builder;
    const unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(str.c_str(), str.c_str() + stringLength, &root,
                       &err)) {
      CLogger::Debug("Failed to parse json string");
      throw runtime_error("Failed to parse json string");
    }
    return root;
  }

  string parseJsonToString(const Json::Value &val) {
    Json::StreamWriterBuilder wbuilder;
    string str_data = writeString(wbuilder, val);
    return str_data;
  }

  Semver parseStringToSemver(string version) {
    auto list = splitStringByDelimiter(version, ".");
    if (list.size() != 3) {
      return Semver{0, 0, 0};
    }

    auto major = stoi(list.front());
    list.pop_front();
    auto minor = stoi(list.front());
    list.pop_front();
    auto patch = stoi(list.front());


    return Semver{major, minor, patch};
  }

  list<string> splitStringByDelimiter(string text, string delimiter) {
    list<string> ls{};

    size_t pos = 0;
    std::string token;
    while ((pos = text.find(delimiter)) != string::npos) {
      token = text.substr(0, pos);
      ls.push_back(token);
      text.erase(0, pos + delimiter.length());
    }
    ls.push_back(text);

    return ls;
  }

}// namespace Assets::Parser