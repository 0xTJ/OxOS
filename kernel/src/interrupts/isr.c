//http://forum.osdev.org/viewtopic.php?p=279939#p279939

#include "isr.h"
#include "pic.h"
#include "idt.h"

#define NULL (0) 		//TEMP

void taskswitch(void * stack) {
	stack = stack;
}

void *(**irqFuncs)(void *ctx);//array of pointers to functions for the IRQs
void *(**intFuncs)(void *ctx);//array of pointers to functions for the interrupts
void *(**intFuncs_err)(void *ctx, uint32_t errcode);//array of pointers to functions for the exceptions with error code

//this function is called in all the IRQs
//it will call the corresponding function in the irqFuncs array, as long as it's not NULL (and the interrupt is not spourious)
//if the called function returns a non-NULL pointer, that pointer will be used as a stack to switch the task
//this function correctly acknowledges normal and spourious hardware interrupts
void irqFunc(uint32_t irqnum, void *ctx)
{
	void *stack = NULL;
	if(picIsNormalIrq(irqnum))
	{
		if(irqFuncs[irqnum] != NULL)
			stack = irqFuncs[irqnum](ctx);
		picSendEOI(irqnum);
		if(stack)
			taskswitch(stack);
	}
	else
	{
		picSendEOISpurious(irqnum);
	}
}
 
//this function is called in all the software interrupts, and in exceptions without error code
//it will call the corresponding function in the intFuncs array, as long as it's not NULL
//if the called function returns a non-NULL pointer, that pointer will be used as a stack to switch the task
void intFunc(uint32_t intnum, void *ctx)
{
	void *stack = NULL;
	if(intFuncs[intnum] != NULL)
		stack = intFuncs[intnum](ctx);
	if(stack)
		taskswitch(stack);
}
 
//this function is called in exceptions with error code
//it will call the corresponding function in the intFuncs_err array, as long as it's not NULL
//if the called function returns a non-NULL pointer, that pointer will be used as a stack to switch the task
void intFuncErr(uint32_t intnum, void *ctx, uint32_t errcode)
{
	void *stack = NULL;
	if(intFuncs_err[intnum] != NULL)
		stack = intFuncs_err[intnum](ctx, errcode);
	if(stack)
		taskswitch(stack);
}

#define DEFIRQWRAPPER(irqnum)\
void *irq##irqnum##handler(void)\
{\
	volatile void *addr;\
	asm goto("jmp %l[endofISR]" ::: "memory" : endofISR);\
	asm volatile(".align 16" ::: "memory");\
	startofISR:\
	asm volatile("pushal\n\tpushl %%ebp\n\tmovl %%esp, %%ebp\n\tcld" ::: "memory");\
	asm volatile(\
		"pushl %%ds       \n\t"\
		"pushl %%es       \n\t"\
		"movw $16, %%cx   \n\t"\
		"movw %%cx, %%ds  \n\t"\
		"movw %%cx, %%es  \n\t"\
		"pushl %%ebp      \n\t"\
		"addl $4, (%%esp) \n\t"\
		"pushl %%ebx      \n\t"\
		"call *%%eax      \n\t"\
		"addl $8, %%esp       "\
		:: "a"(irqFunc), "b"((uint32_t) irqnum) : "memory");\
	asm volatile("popl %%es\n\tpopl %%ds\n\tleave\n\tpopal\n\tiret" ::: "memory");\
	endofISR:\
	asm goto(\
		".intel_syntax noprefix         \n\t"\
		"mov eax, offset %l[startofISR] \n\t"\
		"mov [ebx], eax                 \n\t"\
		".att_syntax                        "\
		:: "b"(&addr) : "eax", "memory" : startofISR);\
	return((void *) addr);\
}

#define DEFINTWRAPPER(intnum)\
void *int##intnum##handler(void)\
{\
	volatile void *addr;\
	asm goto("jmp %l[endofISR]" ::: "memory" : endofISR);\
	asm volatile(".align 16" ::: "memory");\
	startofISR:\
	asm volatile("pushal\n\tpushl %%ebp\n\tmovl %%esp, %%ebp\n\tcld" ::: "memory");\
	asm volatile(\
		"pushl %%ds       \n\t"\
		"pushl %%es       \n\t"\
		"movw $16, %%cx   \n\t"\
		"movw %%cx, %%ds  \n\t"\
		"movw %%cx, %%es  \n\t"\
		"pushl %%ebp      \n\t"\
		"addl $4, (%%esp) \n\t"\
		"pushl %%ebx      \n\t"\
		"call *%%eax      \n\t"\
		"addl $8, %%esp       "\
		:: "a"(intFunc), "b"((uint32_t) intnum) : "memory");\
	asm volatile("popl %%es\n\tpopl %%ds\n\tleave\n\tpopal\n\tiret" ::: "memory");\
	endofISR:\
	asm goto(\
		".intel_syntax noprefix         \n\t"\
		"mov eax, offset %l[startofISR] \n\t"\
		"mov [ebx], eax                 \n\t"\
		".att_syntax                        "\
		:: "b"(&addr) : "eax", "memory" : startofISR);\
	return((void *) addr);\
}

#define DEFINTWRAPPER_ERR(intnum)\
void *int##intnum##handler(void)\
{\
	volatile void *addr;\
	asm goto("jmp %l[endofISR]" ::: "memory" : endofISR);\
	asm volatile(".align 16" ::: "memory");\
	startofISR:\
	asm volatile(\
		"pushal                \n\t"\
		"pushl %%ebp           \n\t"\
		"movl %%esp, %%ebp     \n\t"\
		"pushl %%ds            \n\t"\
		"pushl %%es            \n\t"\
		"movw $16, %%cx        \n\t"\
		"movw %%cx, %%ds       \n\t"\
		"movw %%cx, %%es       \n\t"\
		"movl 36(%%ebp), %%edx \n\t"\
		"movl %%ebp, %%esi     \n\t"\
		"addl $32, %%esi       \n\t"\
		"movl %%esi, %%edi     \n\t"\
		"addl $4, %%edi        \n\t"\
		"movl $11, %%ecx       \n\t"\
		"std                   \n\t"\
		"rep movsl             \n\t"\
		"add $4, %%esp         \n\t"\
		"cld                       "\
		::: "memory");\
	asm volatile(\
		"pushl %%edx       \n\t"\
		"pushl %%ebp       \n\t"\
		"addl $8, (%%esp)  \n\t"\
		"pushl %%ebx       \n\t"\
		"call *%%eax       \n\t"\
		"addl $12, %%esp       "\
		:: "a"(intFuncErr), "b"((uint32_t) intnum) : "memory");\
	asm volatile("popl %%es\n\tpopl %%ds\n\tleave\n\tpopal\n\tiret" ::: "memory");\
	endofISR:\
	asm goto(\
		".intel_syntax noprefix         \n\t"\
		"mov eax, offset %l[startofISR] \n\t"\
		"mov [ebx], eax                 \n\t"\
		".att_syntax                        "\
		:: "b"(&addr) : "eax", "memory" : startofISR);\
	return((void *) addr);\
}

//IRQs (the 16 IRQs the PIC has)
DEFIRQWRAPPER(0)
DEFIRQWRAPPER(1)
DEFIRQWRAPPER(2)
DEFIRQWRAPPER(3)
DEFIRQWRAPPER(4)
DEFIRQWRAPPER(5)
DEFIRQWRAPPER(6)
DEFIRQWRAPPER(7)
DEFIRQWRAPPER(8)
DEFIRQWRAPPER(9)
DEFIRQWRAPPER(10)
DEFIRQWRAPPER(11)
DEFIRQWRAPPER(12)
DEFIRQWRAPPER(13)
DEFIRQWRAPPER(14)
DEFIRQWRAPPER(15)
 
//exceptions without error code
DEFINTWRAPPER(0)//division by 0
DEFINTWRAPPER(1)//debug
DEFINTWRAPPER(2)//NMI
DEFINTWRAPPER(3)//breakpoint
DEFINTWRAPPER(4)//INTO
DEFINTWRAPPER(5)//BOUND
DEFINTWRAPPER(6)//invalid opcode
DEFINTWRAPPER(7)//coprocessor not available
DEFINTWRAPPER(9)//coprocessor segment overrun
DEFINTWRAPPER(16)//coprocessor error
 
//exceptions with error code
DEFINTWRAPPER_ERR(8)//double fault
DEFINTWRAPPER_ERR(10)//TSS error
DEFINTWRAPPER_ERR(11)//segment not present
DEFINTWRAPPER_ERR(12)//stack fault
DEFINTWRAPPER_ERR(13)//general protection fault
DEFINTWRAPPER_ERR(14)//page fault
 
//software interrupts
DEFINTWRAPPER(0x80)//system call

void isrSetup() {
	idtSetEnt(idt+32, 8, (uint32_t) irq0handler(), 0xe, 0);//IRQ handlers
	idtSetEnt(idt+33, 8, (uint32_t) irq1handler(), 0xe, 0);
	idtSetEnt(idt+34, 8, (uint32_t) irq2handler(), 0xe, 0);
	idtSetEnt(idt+35, 8, (uint32_t) irq3handler(), 0xe, 0);
	idtSetEnt(idt+36, 8, (uint32_t) irq4handler(), 0xe, 0);
	idtSetEnt(idt+37, 8, (uint32_t) irq5handler(), 0xe, 0);
	idtSetEnt(idt+38, 8, (uint32_t) irq6handler(), 0xe, 0);
	idtSetEnt(idt+39, 8, (uint32_t) irq7handler(), 0xe, 0);
	idtSetEnt(idt+40, 8, (uint32_t) irq8handler(), 0xe, 0);
	idtSetEnt(idt+41, 8, (uint32_t) irq9handler(), 0xe, 0);
	idtSetEnt(idt+42, 8, (uint32_t) irq10handler(), 0xe, 0);
	idtSetEnt(idt+43, 8, (uint32_t) irq11handler(), 0xe, 0);
	idtSetEnt(idt+44, 8, (uint32_t) irq12handler(), 0xe, 0);
	idtSetEnt(idt+45, 8, (uint32_t) irq13handler(), 0xe, 0);
	idtSetEnt(idt+46, 8, (uint32_t) irq14handler(), 0xe, 0);
	idtSetEnt(idt+47, 8, (uint32_t) irq15handler(), 0xe, 0);
	 
	idtSetEnt(idt+0, 8, (uint32_t) int0handler(), 0xe, 0);//exception handlers
	idtSetEnt(idt+1, 8, (uint32_t) int1handler(), 0xe, 0);
	idtSetEnt(idt+2, 8, (uint32_t) int2handler(), 0xe, 0);
	idtSetEnt(idt+3, 8, (uint32_t) int3handler(), 0xe, 0);
	idtSetEnt(idt+4, 8, (uint32_t) int4handler(), 0xe, 0);
	idtSetEnt(idt+5, 8, (uint32_t) int5handler(), 0xe, 0);
	idtSetEnt(idt+6, 8, (uint32_t) int6handler(), 0xe, 0);
	idtSetEnt(idt+7, 8, (uint32_t) int7handler(), 0xe, 0);
	idtSetEnt(idt+8, 8, (uint32_t) int8handler(), 0xe, 0);
	idtSetEnt(idt+9, 8, (uint32_t) int9handler(), 0xe, 0);
	idtSetEnt(idt+10, 8, (uint32_t) int10handler(), 0xe, 0);
	idtSetEnt(idt+11, 8, (uint32_t) int11handler(), 0xe, 0);
	idtSetEnt(idt+12, 8, (uint32_t) int12handler(), 0xe, 0);
	idtSetEnt(idt+13, 8, (uint32_t) int13handler(), 0xe, 0);
	idtSetEnt(idt+14, 8, (uint32_t) int14handler(), 0xe, 0);
	idtSetEnt(idt+16, 8, (uint32_t) int16handler(), 0xe, 0);
	 
	idtSetEnt(idt+0x80, 8, (uint32_t) int0x80handler(), 0xe, 3);//system call handler
}
