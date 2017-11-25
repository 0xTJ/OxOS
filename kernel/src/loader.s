global _start                   ; the entry symbol for ELF

MAGIC_NUMBER	equ 0x1BADB002     ; define the magic number constant
ALIGN_MODULES	equ 0x00000001      ; tell GRUB to align modules
CHECKSUM		equ -(MAGIC_NUMBER + ALIGN_MODULES)  ; calculate the checksum
								; (magic number + checksum + flags should equal 0)

; section .__mbHeader				; READ ALL OF http://wiki.osdev.org/Grub_Error_13 if things break
align 4                         ; the code must be 4 byte aligned
	dd MAGIC_NUMBER             ; write the magic number to the machine code,
	dd ALIGN_MODULES            ; the flags,
	dd CHECKSUM                 ; and the checksum

section .text:                  ; start of the text (code) section
align 4                         ; the code must be 4 byte aligned

_start:                         ; the loader label (defined as entry point in linker script)
	mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the
												; stack (end of memory area)

	push ebx
	
	extern _init
	call _init
	
	extern kmain
	call kmain
	
	extern _fini
	call _fini
	
.loop:
	jmp .loop                   ; loop forever
	
	
KERNEL_STACK_SIZE equ 4096                  ; size of stack in bytes

section .bss
align 4                                     ; align at 4 bytes
kernel_stack:                               ; label points to beginning of memory
	resb KERNEL_STACK_SIZE                  ; reserve stack for the kernel

	
	
	
; TODO: change to crt0
