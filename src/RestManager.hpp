#pragma once
#include "Assets.hpp"
#include "http_code.hpp"
#include "pch.hpp"
#include "restbed.hpp"
#include "service_error.hpp"
#include "json/json.h"
#include <chrono>


namespace Rest {
  
  using namespace std;
  using namespace restbed;
  
  enum class REST_FLAGS {
    REST_CHECK_FLAGS,
    VERIFY_JWT_ADMIN,
    VERIFY_JWT_USER
  };

  enum class REST_METHODS {
    GET,
    POST,
    PATCH,
    PUT,
    DELETE,
    HEAD,
    MERGE,
    OPTIONS,
    TRACE,
    CONNECT
  };

  int parse_http_code_to_int(HTTP_CODE);
  string parse_http_code_to_string(HTTP_CODE);

  struct RestDirData {
    REST_METHODS method;
    function<void(const shared_ptr<Session>)> func;
    string location;
  };

  struct FilteredRestDirData {
    string location;
    vector<RestDirData> *data;
  };

  class REQUEST {
  public:
    explicit REQUEST(shared_ptr<Session>);
    ~REQUEST();

    string getBody();
    Json::Value getJson();
    
    string getHeader(const string& header, const string& default_value = "") {
      const shared_ptr<const Request> request = this->session->get_request();
      return request->get_header(header, default_value);
    }
  
    template<typename T>
    T getQuery(string query, T default_value) {
      const shared_ptr<const Request> request = this->session->get_request();
      return request->get_query_parameter(query, default_value);
    }
  
    template<typename T>
    T getParameter(string param, T default_value) {
      auto request = this->session->get_request();
      return request->get_path_parameter(param, default_value);
    }
  
    shared_ptr<Session> getRawSession();
  
    struct _subData{
      Assets::Jwt::TokenValue tokenValue;
    } subData;
  private:
    shared_ptr<Session> session;
  };

  class RESPONSE {
  public:
    explicit RESPONSE(shared_ptr<Session>);
    ~RESPONSE();

    void send(HTTP_CODE, const string&);
    
    template<typename T>
    void json(HTTP_CODE code, T& data) {
      auto request = this->session->get_request();
      multimap<string, string> headers;
    
      string http_message = parse_http_code_to_string(code);
    
      Json::Value returnValue;
    
      returnValue["data"] = data;
      returnValue["result"] = true;
      returnValue["status"] = (int)code;
      returnValue["message"] = http_message.c_str();
    
      replace(http_message.begin(), http_message.end(), ' ', '_');
      for (auto & c: http_message) c = toupper(c);
    
      returnValue["code"] = http_message;
      string str_data = Assets::Parser::parseJsonToString(returnValue);
    
      headers.insert(pair<string, string>("Content-Length",
                                          to_string(strlen(str_data.c_str()))));
      session->close(parse_http_code_to_int(code), str_data, headers);
    }
    void setHeader(string, string);

    shared_ptr<Session> getRawSession();
    
  private:
    shared_ptr<Session> session;
    
  };

  class RestManager {
  public:
    static bool Initialize(Service &);
    static bool Shutdown();

    static vector<RestDirData> *dirData;
    static bool isMounted;

    explicit RestManager(string = "/");
    explicit RestManager(function<void(RestManager *)>);
    explicit RestManager(string, function<void(RestManager *)>);
    
    ~RestManager();

    bool Append(REST_METHODS, const function<void(REQUEST, RESPONSE)> &,
                initializer_list<REST_FLAGS> = {});
    bool Append(string, REST_METHODS, const function<void(REQUEST, RESPONSE)> &,
                initializer_list<REST_FLAGS> = {});

  private:
    string dir;
  };

  function<void(shared_ptr<Session>)> WRAP_FUNC(function<void(REQUEST, RESPONSE)>,
                                                initializer_list<REST_FLAGS>, REST_METHODS, const string&);
  void middlewareHandler(REQUEST&, RESPONSE&, const initializer_list<REST_FLAGS>&);
  string parse_method_str(REST_METHODS);

}// namespace Rest