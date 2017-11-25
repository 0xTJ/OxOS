global setGdt

; setGdt - load GDT
; stack: [esp + 8] limit
;        [esp + 4] base
;        [esp    ] return address
gdtr DW 0 ; For limit storage
     DD 0 ; For base storage

setGdt:
	mov   eax, [esp + 4]
	mov   [gdtr + 2], eax
	mov   ax, [esp + 8]
	mov   [gdtr], ax
	lgdt  [gdtr]
	ret

global reloadSegments

reloadSegments:
	; Reload CS register containing code selector:
	jmp   0x08:reload_CS ; 0x08 points at the new code selector
reload_CS:
	; Reload data segment registers:
	mov   ax, 0x10 ; 0x10 points at the new data selector
	mov   ds, ax
	mov   es, ax
	mov   fs, ax
	mov   gs, ax
	mov   ss, ax
	ret
