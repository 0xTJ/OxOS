#include "idt.h"

struct idtDescr idt[STATIC_IDT_SIZE] = {0};

void addIdtDescr(struct idtDescr *target, struct idtDescrVals source) { 
	// Encode the offset
	target->offset_0 = source.offset & 0xFF;
	target->offset_1 = (source.offset >> 8) & 0xFF;

	// Encode the selector 
	target->selector = source.selector;

	// Encode zero
	target->zero = 0x00;

	// Encode type_attr
	target->type_attr = source.type_attr;
}

