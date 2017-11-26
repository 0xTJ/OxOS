#ifndef INCLUDE_KERNEL_GDT_H
#define INCLUDE_KERNEL_GDT_H

#include <stdint.h>
#include "logging.h"

#define STATIC_GDT_SIZE 3

struct accessFlags {
	uint8_t rw : 1;
	uint8_t dc : 1;
	uint8_t exec : 1;
	uint8_t privl : 2;
	uint8_t pres : 1;
	uint8_t size : 1;
	uint8_t gran : 1;
};

struct gdtDescrVals {
	uint32_t base;
	uint32_t limit;
	struct accessFlags extraBits;
};

struct gdtDescr {
	uint8_t limit_0;
	uint8_t limit_1;
	uint8_t base_0;
	uint8_t base_1;
	uint8_t base_2;
	uint8_t access;
	uint8_t limit_2_flags;
	uint8_t base_3;
} __attribute__((packed));

extern struct gdtDescr gdt[STATIC_GDT_SIZE];

void constrGdtDescr(struct gdtDescrVals *target, uint32_t base, uint32_t limit, struct accessFlags const *extraBits);
void addGdtDescr(struct gdtDescr *target, struct gdtDescrVals const *source);

void setGdt(struct gdtDescr *base, uint16_t limit);
void reloadSegments();

#endif	// NOT INCLUDE_KERNEL_GDT_H
