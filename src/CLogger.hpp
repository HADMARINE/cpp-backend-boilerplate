#pragma once

#include "pch.hpp"
#include <cstdarg>
#include <ctime>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif



class CLogger {
public:
  enum class COLOR {
    BLACK = 0, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
    LIGHTGRAY, DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN,
    LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
  };
  static bool isUsing;
  static void RawLogger(const char *, ...);
  static void Info(const char *, ...);
  static void ErrorWithDialog(const char *, ...);
  static void Error(const char *, ...);
  static void Debug(const char *, ...);
  static void Success(const char *, ...);
  static void ClearWindow();
  static std::string getTimeNow();
};