//
// Created by HADMARINE on 2020/09/19.
//

#include "../service_error.hpp"

namespace ServiceError {
  ErrorValue getErrorValue(Error e) {
    switch(e){
      case Error::PAGE_NOT_FOUND:
        return ErrorValue{404, "Page Not Found", "PAGE_NOT_FOUND"};
      case Error::INTERNAL_SERVER_ERROR:
        return ErrorValue{500, "Internal Server Error", "INTERNAL_SERVER_ERROR"};
      case Error::TOO_MANY_REQUESTS:
        return ErrorValue{429, "Too Many Requests", "TOO_MANY_REQUESTS"};
        
      case Error::UNSAFE_ACTION:
        return ErrorValue{500, "Unsafe Action Proceed without [UNSAFE] Parameter.", "UNSAFE_ACTION"};
      case Error::PASSWORD_ENCRYPTION_FAILURE:
        return ErrorValue{500, "Password Encryption failed.", "PASSWORD_ENCRYPTION_FAILURE"};
      case Error::AUTH_TOKEN_INVALID:
        return ErrorValue{403, "Token Invalid", "AUTH_TOKEN_INVALID"};
      case Error::AUTH_TOKEN_EXPIRED:
        return ErrorValue{403, "Token Expired", "AUTH_TOKEN_EXPIRED"};
      case Error::AUTH_TOKEN_RENEW_NEEDED:
        return ErrorValue{403, "Token Renew Needed", "AUTH_TOKEN_RENEW_NEEDED"};
      case Error::AUTH_FAIL:
        return ErrorValue{400, "Authentication failed", "AUTH_FAIL"};
      case Error::AUTH_ACCESS_DENIED:
        return ErrorValue{403, "Access Denied", "AUTH_ACCESS_DENIED"};
      case Error::DATA_PARAMETER_NULL:
        return ErrorValue{400, "Necessary Parameter is null", "DATA_PARAMETER_NULL"};
      case Error::DATA_PARAMETER_INVALID:
        return ErrorValue{400, "Parameter is invalid", "DATA_PARAMETER_INVALID"};
      
      case Error::DB_CREATE_FAIL:
        return ErrorValue{500, "Failed to create document on DB", "DB_CREATE_FAIL"};
      case Error::DB_ALREADY_EXISTS:
        return ErrorValue{409, "Unique data confilct. (Data already exists)", "DB_ALREADY_EXISTS"};
      case Error::DB_NOT_FOUND:
        return ErrorValue{404, "Data not found", "DB_NOT_FOUND"};
      case Error::DB_ERROR:
        return ErrorValue{500, "DB Error" , "DB_ERROR"};
      default:
        CLogger::Error("service_error-getErrorValue : Error value undefined.");
        return ErrorValue{500, "Unknown Error", "INTERNAL_SERVER_ERROR"};
    }
  }
}