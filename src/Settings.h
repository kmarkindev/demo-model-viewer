#pragma once

#define DEMO_NAME "DEMO: Model viewer"
#define WINDOW_FULLSCREEN false

#if WINDOW_FULLSCREEN == true
	#define VIEWPORT_WIDTH 1920
	#define VIEWPORT_HEIGHT 1080
#else
	#define VIEWPORT_WIDTH 800
	#define VIEWPORT_HEIGHT 600
#endif
