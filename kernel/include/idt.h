#ifndef INCLUDE_KERNEL_IDT_H
#define INCLUDE_KERNEL_IDT_H

#include <stdint.h>

#define STATIC_IDT_SIZE 256

struct idtDescrVals {
   uint32_t offset;
   uint16_t selector;
   uint8_t type_attr;
};

struct idtDescr {
   uint16_t offset_0; // offset bits 0..15
   uint16_t selector; // a code segment selector in gdt or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_1; // offset bits 16..31
} __attribute__((packed));

extern struct idtDescr idt[STATIC_IDT_SIZE];

void idtSetEnt(struct idtDescr *target, uint16_t selector, uint32_t offset, uint8_t type, uint8_t priv);
void setIdt(struct idtDescr *base, uint16_t limit);

#endif	// INCLUDE_KERNEL_IDTS_H
