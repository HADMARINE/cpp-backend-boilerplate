#include "Logger.hpp"
#include <Windows.h>

string extSmNum(int num) {
	char buf[10];
	if (num < 10) {
		sprintf_s(buf, sizeof(buf), "0%d", num);
	}
	else {
		sprintf_s(buf, sizeof(buf), "%d", num);
	}
	return buf;
}

void SetConsoleTextColor(int foreground, int background) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		foreground + background * 16);
}

void SetConsoleTextColor(void) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		LIGHTGRAY + BLACK * 16);
}

void SetConsoleTextColor(const char* type) {
	if (type == "time") {
		SetConsoleTextColor(DARKGRAY, BLACK);
	}
	else if (type == "info") {
		SetConsoleTextColor(BLACK, LIGHTGRAY);
	}
	else if (type == "info_i") {
		SetConsoleTextColor(LIGHTGRAY, BLACK);
	}
	else if (type == "error") {
		SetConsoleTextColor(BLACK, RED);
	} 
	else if (type == "error_i") {
		SetConsoleTextColor(RED, BLACK);
	}
	else if (type == "debug") {
		SetConsoleTextColor(BLACK, CYAN);
	}
	else if (type == "debug_i") {
		SetConsoleTextColor(CYAN, BLACK);
	}
	else {
		SetConsoleTextColor();
	}
}

string Logger::getTimeNow(void) {
	time_t timeNow = time(NULL);
	struct tm pLocal;

#if defined(_WIN32) || defined(_WIN64)
	localtime_s(&pLocal, &timeNow);
#else
	localtime_r(&timeNow, &pLocal);
#endif
	if (&pLocal == NULL) {
		return "TIME_SHOW_ERROR";
	}

	return "[" + extSmNum((&pLocal)->tm_year + 1900) + "-" + 
		extSmNum((&pLocal)->tm_mon + 1) + "-" +
		extSmNum((&pLocal)->tm_mday) + " " + extSmNum((&pLocal)->tm_hour) + ":" +
		extSmNum((&pLocal)->tm_min) + ":" + extSmNum((&pLocal)->tm_sec) + "]: ";
}

void Logger::RawLogger(string mes) {
	SetConsoleTextColor("time");
	cout << Logger::getTimeNow();
	SetConsoleTextColor();
	cout << mes << endl;
}

void Logger::Info(string mes) {
	SetConsoleTextColor("time");
	cout << Logger::getTimeNow();
	SetConsoleTextColor(BLACK, LIGHTGRAY);
	cout << " INFO ";
	SetConsoleTextColor();
	cout << " " << mes << endl;
}

void Logger::Error(string mes,
	bool showErrorStackByDialogue) {
	SetConsoleTextColor("time");
	cout << Logger::getTimeNow();
	SetConsoleTextColor("error");
	cout << " ERRR ";
	SetConsoleTextColor("error_i");
	cout << " " << mes << endl;
	SetConsoleTextColor();
	if (showErrorStackByDialogue == true) {
		wchar_t application_name[128] = L"", content[4096] = L"";
		size_t application_name_size, content_size;
		mbstowcs_s(&application_name_size, application_name, 128, APPLICATION_NAME, 128);
		mbstowcs_s(&content_size, content, 4096, mes.c_str(), 4096);
		MessageBox(NULL, LPCWSTR(content),
			LPCWSTR(application_name), MB_OK);
	}
}

void Logger::Debug(string mes) {
	if (ISDEBUGMODE == true) {
		SetConsoleTextColor("time");
		cout << Logger::getTimeNow();
		SetConsoleTextColor("debug");
		cout << " DEBG ";
		SetConsoleTextColor("debug_i");
		cout << " " << mes << endl;
		SetConsoleTextColor();
	}
}

void Logger::ClearWindow() {
	SetConsoleTextColor("time");

	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
	SetConsoleTextColor();
}