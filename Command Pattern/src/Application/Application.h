#pragma once
#include <string.h>

namespace Application {

	static const unsigned int WIDTH = 900;
	static const unsigned int HEIGHT = 600;
	static const unsigned int NUM_PIXELS_ELEMNTS = WIDTH * HEIGHT * 4;
	static unsigned char* data;

	static unsigned int time;

	void Init();
	void Update();
	void Close();
}