#pragma once

#include "stdafx.h"

using namespace std;

class Assets {
public:
	static void pauseUntilKeyPressed(char, string);
	static void pauseUntilKeyPressed(char k);
	static void pauseUntilKeyPressed(string mes);
	static void pauseUntilKeyPressed();
};
