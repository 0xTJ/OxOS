#include <stdio.h>
#include "io.h"
#include "multiboot.h"
#include "fbio.h"
#include "logging.h"
#include "gdt.h"
#include "idt.h"

void setupMem();

int kmain (unsigned int ebx) {
	setupMem();
	
	typedef void (*call_module_t)(void);
	multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
	unsigned int address_of_module = mbinfo->mods_addr;
	call_module_t start_program = *(call_module_t *) address_of_module;
	start_program++; // Until I figure out why the binaries have a header
    // start_program();
	
	while(1)
        fb_printf("%x", inb(0x60));
}

void setupMem() {
	int i;
	struct gdtDescrVals gdtEntries[STATIC_GDT_SIZE];
	
	gdtEntries[0] = (struct gdtDescrVals){.base=0, .limit=0, .type=0};                     // Selector 0x00 cannot be used
	gdtEntries[1] = (struct gdtDescrVals){.base=0, .limit=0xffffffff, .type=0x9A};         // Selector 0x08 will be our code
	gdtEntries[2] = (struct gdtDescrVals){.base=0, .limit=0xffffffff, .type=0x92};         // Selector 0x10 will be our data
	// gdtEntries[3] = (struct gdtEntry){.base=&myTss, .limit=sizeof(myTss), .type=0x89}; // You can use LTR(0x18)
	
	for (i = 0; i < STATIC_GDT_SIZE; i++)
		addGdtDescr(gdt + i, gdtEntries[i]);
	setGdt(gdt, sizeof(gdt) - 1);
	reloadSegments();
	
	struct idtDescrVals idtEntries[STATIC_IDT_SIZE];
	for (i = 0; i < STATIC_IDT_SIZE; i++)
		addIdtDescr(idt + i, idtEntries[i]);
	setIdt(idt, sizeof(idt) - 1);
}
