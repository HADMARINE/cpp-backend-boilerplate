#include "CLogger.hpp"

#define MAX_BUF_SIZE 4096


#if !(defined(_WIN32) || defined(_WIN64))
//#include <unistd.h>
//#include <term.h>
//#include <curses.h>
#endif


bool CLogger::isUsing = false;

string extSmNum(int num) {
	char buf[10];
	if (num < 10) {
#if defined(_WIN32) || defined(_WIN64)
		sprintf_s(buf, sizeof(buf), "0%d", num);
#else
        snprintf(buf, sizeof(buf), "0%d", num);
#endif
	}
	else {
#if defined(_WIN32) || defined(_WIN64)
        sprintf_s(buf, sizeof(buf), "%d", num);
#else
        snprintf(buf, sizeof(buf), "%d", num);
#endif
	}
	return buf;
}

void SetConsoleTextColor(int foreground, int background) {
#if defined(_WIN32) || defined(_WIN64)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		foreground + background * 16);
#endif
}

void SetConsoleTextColor(void) {
#if defined(_WIN32) || defined(_WIN64)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		LIGHTGRAY + BLACK * 16);
#endif
}

void SetConsoleTextColor(string type) {
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

string CLogger::getTimeNow() {
	time_t timeNow = time(nullptr);
	struct tm pLocal{};

#if defined(_WIN32) || defined(_WIN64)
	localtime_s(&pLocal, &timeNow);
#else
	localtime_r(&timeNow, &pLocal);
#endif
	if (&pLocal == nullptr) {
		return "TIME_SHOW_ERROR";
	}

	return "[" + extSmNum((&pLocal)->tm_year + 1900) + "-" +
		extSmNum((&pLocal)->tm_mon + 1) + "-" +
		extSmNum((&pLocal)->tm_mday) + " " + extSmNum((&pLocal)->tm_hour) + ":" +
		extSmNum((&pLocal)->tm_min) + ":" + extSmNum((&pLocal)->tm_sec) + "] ";
}

void CLogger::RawLogger(const char * mes, ...) {
	while (CLogger::isUsing);
	CLogger::isUsing = true;

    va_list ap;
    char buf[MAX_BUF_SIZE];

    va_start(ap, mes);
    vsprintf(buf, mes, ap);
    va_end(ap);

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTextColor("time");
	cout << CLogger::getTimeNow();
	SetConsoleTextColor();
	cout << buf << endl;
#else
	fprintf(stdout, "%s\x1b[0m%s\n", CLogger::getTimeNow().c_str(), buf);
#endif
	CLogger::isUsing = false;
}

void CLogger::Info(const char* mes, ...) {
	while (CLogger::isUsing);
	CLogger::isUsing = true;

    va_list ap;
    char buf[MAX_BUF_SIZE];

    va_start(ap, mes);
    vsprintf(buf, mes, ap);
    va_end(ap);

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTextColor("time");
	cout << CLogger::getTimeNow();
	SetConsoleTextColor(BLACK, LIGHTGRAY);
	cout << " INFO ";
	SetConsoleTextColor();
	cout << " " << buf << endl;
#else
    fprintf(stdout, "%s\x1b[47;30m INFO \x1b[0m %s\n", CLogger::getTimeNow().c_str(), buf);
#endif
	CLogger::isUsing = false;
}

void CLogger::ErrorWithDialog(const char *mes, ...) {
	while (CLogger::isUsing);
    CLogger::isUsing = true;

	va_list ap;
	char buf[MAX_BUF_SIZE];

    va_start(ap, mes);
    vsprintf(buf, mes, ap);
    va_end(ap);

#if defined(_WIN32) || defined(_WIN64)
	SetConsoleTextColor("time");
	cout << CLogger::getTimeNow();
	SetConsoleTextColor("error");
	cout << " ERRR ";
	SetConsoleTextColor("error_i");
    cout << buf << endl;
	SetConsoleTextColor();
#else
	fprintf(stderr,"%s\x1b[30;41m ERRR \x1b[31;40m %s\x1b[0m\n",
         CLogger::getTimeNow().c_str(), buf);
#endif

#if defined(_WIN32) || defined(_WIN64)
		wchar_t application_name[128] = L"", content[4096] = L"";
		size_t application_name_size, content_size;
		mbstowcs_s(&application_name_size, application_name, 128, APPLICATION_NAME, 128);
		mbstowcs_s(&content_size, content, 4096, mes.c_str(), 4096);
		MessageBox(NULL, LPCWSTR(content),
			LPCWSTR(application_name), MB_OK);
#else
        CLogger::Debug("CLogger::Error - ErrorStackDialogue is avaliable only in WINDOWS");
#endif
	CLogger::isUsing = false;
}

void CLogger::Error(const char* mes, ...) {
    while (CLogger::isUsing);
    CLogger::isUsing = true;

    va_list ap;
    char buf[MAX_BUF_SIZE];

    va_start(ap, mes);
    vsprintf(buf, mes, ap);
    va_end(ap);

#if defined(_WIN32) || defined(_WIN64)
    SetConsoleTextColor("time");
	cout << CLogger::getTimeNow();
	SetConsoleTextColor("error");
	cout << " ERRR ";
	SetConsoleTextColor("error_i");
    cout << buf << endl;
	SetConsoleTextColor();
#else
    fprintf(stderr,"%s\x1b[30;41m ERRR \x1b[31;40m %s\x1b[0m\n",
            CLogger::getTimeNow().c_str(), buf);
#endif
    CLogger::isUsing = false;
}

void CLogger::Debug(const char * mes, ...) {
	if (!ISDEBUGMODE) {
		return;
	}
	
	while (CLogger::isUsing);
	CLogger::isUsing = true;

    va_list ap;
    char buf[MAX_BUF_SIZE];

    va_start(ap, mes);
    vsprintf(buf, mes, ap);
    va_end(ap);

#if defined(_WIN32) || defined(_WIN64)
	SetConsoleTextColor("time");
	cout << CLogger::getTimeNow();
	SetConsoleTextColor("debug");
	cout << " DEBG ";
	SetConsoleTextColor("debug_i");
	cout << " " << mes << endl;
	SetConsoleTextColor();
#else
	fprintf(stdout, "%s\x1b[30;46m DEBG \x1b[36;40m %s\x1b[0m\n", CLogger::getTimeNow().c_str(), buf);
#endif

	CLogger::isUsing = false;
}

void CLogger::ClearWindow() {
	while (CLogger::isUsing);
	CLogger::isUsing = true;
	SetConsoleTextColor("time");

#if defined(_WIN32) || defined(_WIN64)
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
	CLogger::isUsing = false;
#else
//    if (!cur_term)
//    {
//        int result;
//        setupterm( nullptr, STDOUT_FILENO, &result );
//        if (result <= 0) return;
//    }
//
//    putp(tigetstr("clear"));
    //TODO : Cross Platform
    cout << string(20, '\n');
#endif
}