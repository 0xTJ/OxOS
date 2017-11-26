#include "idt.h"

struct idtDescr idt[STATIC_IDT_SIZE] = {0};

void idtSetEnt(struct idtDescr *target, uint16_t selector, uint32_t offset, uint8_t type, uint8_t priv) { 
	// Encode the offset
	target->offset_0 = offset & 0xFF;
	target->offset_1 = (offset >> 8) & 0xFF;

	// Encode the selector 
	target->selector = selector;

	// Encode zero
	target->zero = 0x00;

	// Encode type_attr
	target->type_attr = (1 << 7) | (priv << 5) | type;
}
