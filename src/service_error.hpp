//
// Created by HADMARINE on 2020/09/19.
//

#ifndef CPP_BACKEND_BOILERPLATE_SERVICE_ERROR_HPP
#define CPP_BACKEND_BOILERPLATE_SERVICE_ERROR_HPP

#include "stdafx.h"
#include <json/json.h>
#include "Parser.hpp"

namespace ServiceError {
  enum class Error {
    PAGE_NOT_FOUND,
    INTERNAL_SERVER_ERROR,
    TOO_MANY_REQUESTS,
    
    UNSAFE_ACTION,
    
    PASSWORD_ENCRYPTION_FAILURE,
    AUTH_TOKEN_INVALID,
    AUTH_TOKEN_EXPIRED,
    AUTH_TOKEN_RENEW_NEEDED,
    AUTH_FAIL,
    AUTH_ACCESS_DENIED,
    DATA_PARAMETER_NULL,
    DATA_PARAMETER_INVALID,
    
    DB_CREATE_FAIL,
    DB_ALREADY_EXISTS,
    DB_NOT_FOUND,
    DB_ERROR,
    
//    AWS_SES_FAIL,
//    AWS_S3_FAIL
  };
  
  struct ErrorValue {
    int status;
    string message;
    string code;
  };
  
  ErrorValue getErrorValue(Error);
  Json::Value ErrorToJson(Error);
}



#endif//CPP_BACKEND_BOILERPLATE_SERVICE_ERROR_HPP
