#ifndef INCLUDE_KERNEL_gdt_H
#define INCLUDE_KERNEL_gdt_H

#include <stdint.h>
#include "logging.h"

#define STATIC_GDT_SIZE 3

struct gdtDescrVals {
	uint32_t base;
	uint32_t limit;
	uint8_t type;
};

struct gdtDescr {
	uint8_t limit_0;
	uint8_t limit_1;
	uint8_t base_0;
	uint8_t base_1;
	uint8_t base_2;
	uint8_t type;
	uint8_t limit_2_flags;
	uint8_t base_3;
} __attribute__((packed));

extern struct gdtDescr gdt[STATIC_GDT_SIZE];

/**
 * \param target A pointer to the 8-byte gdt entry
 * \param source An arbitrary structure describing the gdt entry
 */
void addGdtDescr(struct gdtDescr *target, struct gdtDescrVals source);

void setGdt(struct gdtDescr *base, uint16_t limit);
void reloadSegments();

#endif	// NOT INCLUDE_KERNEL_gdt_H
