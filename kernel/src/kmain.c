#include <stdio.h>
#include "io.h"
#include "multiboot.h"
#include "fbio.h"
#include "logging.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"

void setupMem();

// struct interrupt_frame {
    // uint32_t not_preserved_register_1;
    // uint32_t special_interrupt_register;
// };
// __attribute__((interrupt)) void interrupt_handler(struct interrupt_frame* frame) {
	// frame = frame;
    // printf("INTED");
// }

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
	
	debugLog(LOG_INFO, "Creating GDT structs");
	constrGdtDescr(gdtEntries + 0, 0, 0, &(struct accessFlags) { .rw = 0, .dc = 0, .exec = 0, .privl = 0, .pres = 0, .size = 0 });	// Selector 0x00 cannot be used
	constrGdtDescr(gdtEntries + 1, 0, 0xffffffff, &(struct accessFlags) { .rw = 1, .dc = 0, .exec = 1, .privl = 0, .pres = 1, .size = 1 });	// Selector 0x08 will be 0 code
	constrGdtDescr(gdtEntries + 2, 0, 0xffffffff, &(struct accessFlags) { .rw = 1, .dc = 0, .exec = 0, .privl = 0, .pres = 1, .size = 1 });	// Selector 0x10 will be 0 data
	constrGdtDescr(gdtEntries + 3, 0, 0xffffffff, &(struct accessFlags) { .rw = 1, .dc = 0, .exec = 1, .privl = 3, .pres = 1, .size = 1 });	// Selector 0x08 will be 3 code
	constrGdtDescr(gdtEntries + 4, 0, 0xffffffff, &(struct accessFlags) { .rw = 1, .dc = 0, .exec = 0, .privl = 3, .pres = 1, .size = 1 });	// Selector 0x10 will be 3 data
	gdtEntries[3] = (struct gdtDescrVals){.base=&tss, .limit=sizeof(tss), .type=0x89}; // You can use ltr 0x18
	for (i = 0; i < STATIC_GDT_SIZE; i++)
		addGdtDescr(gdt + i, gdtEntries + i);
	setGdt(gdt, sizeof(gdt) - 1);
	reloadSegments();
	
	// struct idtDescrVals idtEntries[STATIC_IDT_SIZE];
	// idtEntries[80].offset = (uint32_t) int80handler;
	// idtEntries[80].selector = 1;
	// for (i = 0; i < STATIC_IDT_SIZE; i++)
		// addIdtDescr(idt + i, idtEntries[i]);
	setIdt(idt, sizeof(idt) - 1);
	
	// asm ("int 80");
}
