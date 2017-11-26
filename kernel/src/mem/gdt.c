#include "gdt.h"
#include <stdio.h>

struct gdtDescr gdt[STATIC_GDT_SIZE] = {0};

void constrGdtDescr(struct gdtDescrVals *target, uint32_t base, uint32_t limit, struct accessFlags const *extraBits) {
	target->base = base;
	target->extraBits = *extraBits;
	
    if ((limit > 65536) && ((limit & 0xFFF) != 0xFFF)) {
        debugLog(LOG_ERROR, "Large segments must be aligned to 4k blocks (0xFFF).");
		return;
    }
    if (limit > 65536) {
        // Adjust granularity if required
        target->limit = limit >> 12;
        target->extraBits.gran = 1;
    }
	else {
        target->limit = limit;
        target->extraBits.gran = 0;
    }
}

void addGdtDescr(struct gdtDescr *target, struct gdtDescrVals const *source) {
    // Check the limit to make sure that it can be encoded
 
    // Encode the limit
    target->limit_0 = source->limit & 0xFF;
    target->limit_1 = (source->limit >> 8) & 0xFF;
    target->limit_2_flags = (source->limit >> 16) & 0x0F;

    // Encode the base 
    target->base_0 = source->base & 0xFF;
    target->base_1 = (source->base >> 8) & 0xFF;
    target->base_2 = (source->base >> 16) & 0xFF;
    target->base_3 = (source->base >> 24) & 0xFF;
 
    // And... Type
	target->access = 0b00010000;
	target->access |= (source->extraBits.pres << 7) & 0x80;
	target->access |= (source->extraBits.privl << 5) & 0x60;
	target->access |= (source->extraBits.exec << 3) & 0x08;
	target->access |= (source->extraBits.dc << 2) & 0x04;
	target->access |= (source->extraBits.rw << 1) & 0x02;
	
	
	target->limit_2_flags |= (source->extraBits.gran << 7) & 0x80;
	target->limit_2_flags |= (source->extraBits.size << 6) & 0x40;
}
