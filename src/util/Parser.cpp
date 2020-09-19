#include "../Parser.hpp"


wstring Parser::parseStringToWstring(const string str) {
	wstring* wstr = new wstring;
	wstr->assign(str.begin(), str.end());
	return *wstr;
};