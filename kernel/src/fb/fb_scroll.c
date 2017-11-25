#include "fbio.h"
#include <stdio.h>
#include "logging.h"

void fb_scroll(int n) {
	int i, j;
	int posDec;
	
	if (n > 0) {
		for (i = 0; i < FB_SCREEN_HEIGHT; i++) {
			if (i + n < FB_SCREEN_HEIGHT) {
				for (j = 0; j < FB_SCREEN_WIDTH; j++) {
					fb[(i * FB_SCREEN_WIDTH + j) * 2] = fb[((i + n) * FB_SCREEN_WIDTH + j) * 2];
					fb[(i * FB_SCREEN_WIDTH + j) * 2 + 1] = fb[((i + n) * FB_SCREEN_WIDTH + j) * 2 + 1];
				}
			}
			else {
				for (j = 0; j < FB_SCREEN_WIDTH; j++) {
					fb[(i * FB_SCREEN_WIDTH + j) * 2] = 0;
					fb[(i * FB_SCREEN_WIDTH + j) * 2 + 1] = ((curFg & 0x0F) << 4) | (curBg & 0x0F);
				}
			}
		}
	}
	else if (n < 0) {
		for (i = FB_SCREEN_HEIGHT - 1; i >= 0; i--) {
			if (i + n >= 0) {
				for (j = 0; j < FB_SCREEN_WIDTH; j++) {
					fb[(i * FB_SCREEN_WIDTH + j) * 2] = fb[((i + n) * FB_SCREEN_WIDTH + j) * 2];
					fb[(i * FB_SCREEN_WIDTH + j) * 2 + 1] = fb[((i + n) * FB_SCREEN_WIDTH + j) * 2 + 1];
				}
			}
			else {
				for (j = 0; j < FB_SCREEN_WIDTH; j++) {
					fb[(i * FB_SCREEN_WIDTH + j) * 2] = 0;
					fb[(i * FB_SCREEN_WIDTH + j) * 2 + 1] = ((curFg & 0x0F) << 4) | (curBg & 0x0F);
				}
			}
		}
	}
	
	posDec = n * FB_SCREEN_WIDTH;
	if (curPos - posDec <= 1)
		curPos = 0;
	else if (curPos - posDec >= FB_SCREEN_WIDTH * FB_SCREEN_HEIGHT)
		curPos = FB_SCREEN_WIDTH * (FB_SCREEN_HEIGHT - 1);
	else
		curPos -= posDec;
	fb_move_cursor(curPos);
}
