#include "Assets.hpp"

void Assets::pauseUntilKeyPressed(char k, string mes) {
	CLogger::Info(mes);
	fgetc(stdin);
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Assets::pauseUntilKeyPressed(char k) {
	fgetc(stdin);
	cin.ignore(numeric_limits<streamsize>::max(), k);
}

void Assets::pauseUntilKeyPressed(string mes) {
	CLogger::Info(mes);
	fgetc(stdin);
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Assets::pauseUntilKeyPressed(void) {
	fgetc(stdin);
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}