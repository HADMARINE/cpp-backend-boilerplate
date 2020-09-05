#pragma once
#include "stdafx.h"

using namespace std;

class Assets {
public:
	static void pauseUntilKeyPressed(char k, string mes) {
		Logger::Info(mes);
		fgetc(stdin);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	static void pauseUntilKeyPressed(char k) {
		fgetc(stdin);
		cin.ignore(numeric_limits<streamsize>::max(), k);
	}
	static void pauseUntilKeyPressed(string mes) {
		Logger::Info(mes);
		fgetc(stdin);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	static void pauseUntilKeyPressed() {
		fgetc(stdin);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
};