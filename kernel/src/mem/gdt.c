#include "gdt.h"

struct gdtDescr gdt[STATIC_GDT_SIZE] = {0};

void addGdtDescr(struct gdtDescr *target, struct gdtDescrVals source) {
    // Check the limit to make sure that it can be encoded
    if ((source.limit > 65536) && ((source.limit & 0xFFF) != 0xFFF)) {
        debugLog(LOG_ERROR, "You can't use that in a gdt!");
		return;
    }
    if (source.limit > 65536) {
        // Adjust granularity if required
        source.limit = source.limit >> 12;
        target->limit_2_flags = 0xC0;
    } else {
        target->limit_2_flags = 0x40;
    }
 
    // Encode the limit
    target->limit_0 = source.limit & 0xFF;
    target->limit_1 = (source.limit >> 8) & 0xFF;
    target->limit_2_flags |= (source.limit >> 16) & 0xF;
 
    // Encode the base 
    target->base_0 = source.base & 0xFF;
    target->base_1 = (source.base >> 8) & 0xFF;
    target->base_2 = (source.base >> 16) & 0xFF;
    target->base_3 = (source.base >> 24) & 0xFF;
 
    // And... Type
    target->type = source.type;
}
