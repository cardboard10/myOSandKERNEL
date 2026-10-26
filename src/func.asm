; return: uint8_t then al
; return: uint32_t then eax
; return: uint64_t then rax

;args [RCX,RDX,R8,R9]



global diskWriteFlush
global prepareDiskRead
global diskReadByte
global prepareDiskWrite
global diskWriteByte
global getkeydata
global getkeystatus
global readReg
global writeReg

section .text
;uint8_t getkeydata()->data
getkeydata:
	mov dx, 0x60
	in al, dx
	ret
;uint8_t getkeystatus()->status
getkeystatus:
	mov dx, 0x64
	in al, dx
	ret
;uint8_t readReg(reg)->data
readReg:
	mov dx, cx
	in al, dx
	ret
;uint8_t writeReg(reg,data)
writeReg:
	mov al, dl
	mov dx, cx
	out dx, al
	ret

;void prepareDiskRead(addrP1,addrP2,addrP3)
prepareDiskRead:
	;wait till 1F7.7==0
.moveArgs:
	mov R10B, CL
	mov R11B, DL
	mov R12B, R8B
.waitTillReady:
	mov ax, 0
	mov dx, 0x1F7
	in al, dx
	bt ax, 7
	jc .waitTillReady
.selectDrive:
	mov al, 0b11110000
	mov dx, 0x1F6
	out dx, al
.setSectorCount:
	mov al,0x1
	mov dx, 0x1F2
	out dx,al
.setReadAddr:
	mov al, R10B
	mov dx, 0x1F3
	out dx,al

	mov al, R11B
	mov dx, 0x1F4
	out dx,al

	mov al, R12B
	mov dx, 0x1F5
	out dx,al
.setMode:
	mov dx, 0x1F7
	mov al, 0x20 ;?
	out dx,al
.waitTillReady2:
	mov ax, 0
	mov dx, 0x1F7
	in al, dx
	bt ax, 7
	jc .waitTillReady2
	bt ax, 3
	jnc .waitTillReady2
	ret
; uint16_t diskReadByte()->uint16_t
diskReadByte:
	mov ax, 0
	mov dx, 0x1F0
	in ax, dx
	ret


;void prepareDiskWrite(addrP1,addrP2,addrP3)
prepareDiskWrite:
	;wait till 1F7.7==0
.moveArgs:
	mov R10B, CL
	mov R11B, DL
	mov R12B, R8B
.waitTillReady:
	mov ax, 0
	mov dx, 0x1F7
	in al, dx
	bt ax, 7
	jc .waitTillReady
.selectDrive:
	mov al, 0b11110000
	mov dx, 0x1F6
	out dx, al
.setSectorCount:
	mov al,0x1
	mov dx, 0x1F2
	out dx,al
.setWriteAddr:
	mov al, R10B
	mov dx, 0x1F3
	out dx,al

	mov al, R11B
	mov dx, 0x1F4
	out dx,al

	mov al, R12B
	mov dx, 0x1F5
	out dx,al
.setMode:
	mov dx, 0x1F7
	mov al, 0x30 ;?
	out dx,al
.waitTillReady2:
	mov ax, 0
	mov dx, 0x1F7
	in al, dx
	bt ax, 7
	jc .waitTillReady2
	bt ax, 3
	jnc .waitTillReady2
	ret
; void diskWriteByte(uint16_t)
diskWriteByte:
	mov dx, 0x1F0
	out dx, ax
	ret
; void diskWriteByte()
diskWriteFlush:
	mov dx, 0x1F7
	mov al, 0xE7 ;?
	out dx,al
	ret