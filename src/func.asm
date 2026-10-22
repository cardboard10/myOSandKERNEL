; return: uint8_t then al
; return: uint32_t then eax
; return: uint64_t then rax

;args [RCX,RDX,R8,R9]




global getkeydata
global getkeystatus

section .text
;uint8_t getkeydata()
getkeydata:
	mov dx, 0x60
	in al, dx
	ret
;uint8_t getkeydata()
getkeystatus:
	mov dx, 0x64
	in al, dx
	ret