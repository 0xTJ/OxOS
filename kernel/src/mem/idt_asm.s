global setIdt

; setIdt - load IDT
; stack: [esp + 8] limit
;        [esp + 4] base
;        [esp    ] return address
idtr DW 0 ; For size storage
     DD 0 ; For base storage

setIdt:
	mov   eax, [esp + 4]
	mov   [idtr + 2], eax
	mov   ax, [esp + 8]
	mov   [idtr], ax
	lgdt  [idtr]
	ret
