#include "pic.h"
#include "io.h"
 
void picSendEOI(unsigned char irq) {
	if(irq >= 8)
		outb(PIC2_CMD,picEOI);
 
	outb(PIC1_CMD,picEOI);
}

void picSendEOISpurious(unsigned char irq){
	if(irq >= 8)
		outb(PIC2_CMD,picEOI);
 
	outb(PIC1_CMD,picEOI);
}
