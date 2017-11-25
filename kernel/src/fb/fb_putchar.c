#include "fbio.h"

int fb_putchar(int ch) {	// TODO: Check if past buffer size
	while (curPos >= FB_SCREEN_WIDTH * FB_SCREEN_HEIGHT)
		fb_scroll(1);
	fb_write_cell(curPos * 2, ch, curFg, curBg);
	curPos++;
	fb_move_cursor(curPos);
	return 0;
}
