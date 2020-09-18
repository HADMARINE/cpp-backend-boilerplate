#pragma once
#include "stdafx.h"
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif



using namespace std;

enum COLOR {
	BLACK = 0, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
	LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN,
	LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
};

class CLogger {
public:
	static bool isUsing;
	static void RawLogger(string);
	static void Info(string);
	static void Error(string, bool showErrorStackByDialogue = false);
	static void Debug(string);
	static void ClearWindow();
	static string getTimeNow(void);
};
