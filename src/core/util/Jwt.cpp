//
// Created by HADMARINE on 2020/09/28.
//

#include "../../Assets.hpp"
#include "../../pch.hpp"

using namespace std;

namespace Assets::Jwt {
  string create(const JwtSignOptions &options, const Json::Value &payload) {
    auto token = jwt::create()
                     .set_issuer(options.issuer)
                     .set_issued_at(std::chrono::system_clock::now())
                     .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds(timeParser(options.expiresIn)))
                     .set_id(options.jwtid);
    //TODO : add payload data - userid, user doc id, tokenType, authority
    for (auto const &key : payload.getMemberNames()) {
      auto value = payload[key].asString();
      token.set_payload_claim(key, jwt::claim(value));
    }

    return token.sign(jwt::algorithm::hs256(GlobalPreferences::JWT_TOKEN_KEY));
  }

  TokenValue verify(string token, TokenType type) {
    auto verifier =
        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256(
                GlobalPreferences::JWT_TOKEN_KEY))
            .with_issuer(GlobalPreferences::JWT_ISSUER);
    auto decodedRawToken = decode_raw(token);
    verifier.verify(decodedRawToken);
    return TokenValue{
        decodedRawToken.get_payload_claims(),
        decodedRawToken.get_header_claims()};
  }

  TokenValue decode(string token) {
    auto decoded = jwt::decode(token);
    TokenValue value;
    value.header = decoded.get_header_claims();
    value.payload = decoded.get_payload_claims();
    return value;
  }

  jwt::decoded_jwt decode_raw(string token) {
    return jwt::decode(token);
  }

  long long timeParser(basic_string<char, char_traits<char>, allocator<char>> timeStr) {
    char timeIndicator = timeStr.back();
    long long multiply = 1;
    if (!(timeIndicator < 'A' || timeIndicator > 'z')) {
      timeStr.pop_back();
      switch (timeIndicator) {
        case 's':
          multiply = 1000;
          break;
        case 'm':
          multiply = 1000 * 60;
          break;
        case 'h':
          multiply = 1000 * 60 * 60;
          break;
        case 'd':
          multiply = 1000 * 60 * 60 * 24;
          break;
        case 'w':
          multiply = 1000 * 60 * 60 * 24 * 7;
          break;
        case 'M':
          multiply = 1000 * 60 * 60 * 24 * (long long)30;
          break;
        case 'y':
          multiply = 1000 * 60 * 60 * 24 * (long long)365;
          break;
        default:
          CLogger::Debug("Error : Jwt::timeParser has faced unknown time indicator - returning ZeroValue");
          multiply = 0;
          break;
      }
    }
    return stoi(timeStr) * multiply;
  }
}// namespace Assets::Jwt