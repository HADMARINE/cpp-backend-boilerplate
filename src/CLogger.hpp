#pragma once

#include <ctime>
#include <cstdarg>
#include <string>
#include "stdafx.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

enum COLOR {
	BLACK = 0, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
	LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN,
	LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
};

class CLogger {
public:
	static bool isUsing;
	static void RawLogger(const char *, ...);
	static void Info(const char *, ...);
	static void ErrorWithDialog(const char *, ...);
	static void Error(const char *, ...);
	static void Debug(const char *, ...);
	static void ClearWindow();
	static std::string getTimeNow();
};