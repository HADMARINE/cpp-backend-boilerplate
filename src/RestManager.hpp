#pragma once
#include "Assets.hpp"
#include "http_code.hpp"
#include "restbed.hpp"
#include "stdafx.h"
#include "json/json.h"
#include <chrono>
#include "Parser.hpp"
#include "service_error.hpp"

using namespace std;
using namespace restbed;

namespace Rest {
  enum class REST_FLAGS {
    REST_CHECK_FLAGS,
    IS_ABSOLUTE,
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
    template<typename T> T getHeader(string, T);
    template<typename T> T getQuery(string, T);
    template<typename T> T getParameter(string, T);

    shared_ptr<Session> getRawSession();

  private:
    shared_ptr<Session> session;
  };

  class RESPONSE {
  public:
    explicit RESPONSE(shared_ptr<Session>);
    ~RESPONSE();

    void send(HTTP_CODE, const string&);
    void json(HTTP_CODE, Json::Value);

    shared_ptr<Session> getRawSession();

  private:
    shared_ptr<Session> session;
  };

  class RestDirCollector {
  public:
    static bool Initialize(Service &);
    static bool Shutdown();

    static vector<RestDirData> *dirData;
    static bool isMounted;

    explicit RestDirCollector(string = "/");
    explicit RestDirCollector(function<void(RestDirCollector*)>);
    explicit RestDirCollector(string, function<void(RestDirCollector*)>);
    
    ~RestDirCollector();

    bool Append(REST_METHODS, const function<void(REQUEST, RESPONSE)> &,
                initializer_list<REST_FLAGS> = {});
    bool Append(string, REST_METHODS, const function<void(REQUEST, RESPONSE)> &,
                initializer_list<REST_FLAGS> = {});

  private:
    string dir;
  };

  function<void(shared_ptr<Session>)> WRAP_FUNC(function<void(REQUEST, RESPONSE)>,
                                                initializer_list<REST_FLAGS>, REST_METHODS, string);

  string parse_method_str(REST_METHODS);

}// namespace Rest