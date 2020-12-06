#pragma once

#include "pch.hpp"
#include <json/json.h>
#include <jwt-cpp/jwt.h>


namespace Assets {
  void pauseUntilKeyPressed(char, std::string);
	void pauseUntilKeyPressed(char k);
	void pauseUntilKeyPressed(std::string mes);
	void pauseUntilKeyPressed();
  
  namespace Parser {
    struct Semver {
      int major;
      int minor;
      int patch;
    };
    
    std::wstring parseStringToWstring(const std::string &);
    Json::Value parseStringToJson(const std::string &);
    std::string parseJsonToString(const Json::Value &);
    Semver parseStringToSemver(std::string);
    
    std::list<std::string> splitStringByDelimiter(std::string, std::string);
  }
  
  namespace Time {
  
  }
  
  namespace Encrypt {
  
  }
  
  namespace Jwt {
    struct TokenValue {
      std::unordered_map<std::basic_string<char, std::char_traits<char>, std::allocator<char>>, jwt::claim> payload;
      std::unordered_map<std::basic_string<char, std::char_traits<char>, std::allocator<char>>, jwt::claim> header;
    };
    
    struct JwtSignOptions{
      std::string expiresIn;
      std::string jwtid;
      std::string issuer;
    };
    
    struct InitialTokenPayload {
      std::string access;
      std::string refresh;
    };
    
    enum class TokenType {
      ACCESS,
      REFRESH,
      ANY
    };
    
    long long timeParser(std::basic_string<char, std::char_traits<char>, std::allocator<char>>);
    TokenValue verify(std::string, TokenType = TokenType::ANY);
    TokenValue decode(std::string);
    jwt::decoded_jwt decode_raw(std::string token);
    std::string create(const JwtSignOptions&, const Json::Value&);
  }
};
