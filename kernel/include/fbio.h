#ifndef INCLUDE_KERNEL_FB_H
#define INCLUDE_KERNEL_FB_H

#include <stdarg.h>

#define FB_SCREEN_WIDTH 80
#define FB_SCREEN_HEIGHT 25

static char * const fb = (char *) 0x000B8000;
extern int curPos;
extern unsigned char curFg;
extern unsigned char curBg;

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos);

void fb_scroll(int n);

int fb_printf(const char *format, ...);
int fb_vprintf(const char *format, va_list arg);
int fb_putchar(int ch);
int fb_puts(const char *str);

#endif	// NOT INCLUDE_KERNEL_FB_H
