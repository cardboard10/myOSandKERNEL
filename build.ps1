echo "building program"
cd C:\Users\lando\projs\OS\UEFI1\src
echo " 1. Clean up old build files"
Remove-Item main.o, BOOTX64.EFI -ErrorAction SilentlyContinue
echo " 2. Compile main.c"
echo "compiling
--------------------------------------------------------------------------------------------------------------------------------------------"
echo "asm..."
nasm -f win64 func.asm -o func.o
echo "C..."
gcc -ffreestanding -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -I"C:\Users\lando\projs\OS\UEFI1\src\efi-headers\" -I"C:\Users\lando\projs\OS\UEFI1\src\myheaders\" -c main.c -o main.o
echo "finished"
echo "--------------------------------------------------------------------------------------------------------------------------------------------"
echo " 3. Link it as a UEFI application with explicit entry point"
cmd /c "gcc -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -Wl,-e,efi_main -o BOOTX64.EFI main.o func.o -lgcc" 
echo " 4. Move it into your ESP boot folder"
Move-Item -Path BOOTX64.EFI -Destination ..\esp\EFI\BOOT\BOOTX64.EFI -Force
rm main.o
rm func.o
cd C:\Users\lando\projs\OS\UEFI1