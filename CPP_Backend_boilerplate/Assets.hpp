#pragma once

#ifndef ASSETS
#define ASSETS

#include "stdafx.h"
#include ""

using namespace std;

class Assets {
public:
	static void pauseUntilKeyPressed(char, string);
	static void pauseUntilKeyPressed(char k);
	static void pauseUntilKeyPressed(string mes);
	static void pauseUntilKeyPressed(void);
};

#endif