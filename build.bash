#!/bin/bash

# Exit immediately if any command fails
set -e

# Get the root directory where this script lives
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="$ROOT/src"

echo "building program"
cd "$SRC_DIR"

echo " 1. Clean up old build files"
rm -f main.o BOOTX64.EFI

echo " 2. Compile main.c"
echo "compiling----------------------------------------------------------------------------------------------------------------------------"
echo "asm..."
nasm -f win64 func.asm -o func.o

echo "C..."
gcc -ffreestanding -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -I"$SRC_DIR/efi-headers" -I"$SRC_DIR/myheaders" -c main.c -o main.o
echo "finished"
echo "----------------------------------------------------------------------------------------------------------------------------"

echo " 3. Link it as a UEFI application with explicit entry point"
gcc -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -Wl,-e,efi_main -o BOOTX64.EFI main.o func.o -lgcc

echo " 4. Move it into your ESP boot folder"
DEST_DIR="$ROOT/esp/EFI/BOOT"
mkdir -p "$DEST_DIR"
mv -f BOOTX64.EFI "$DEST_DIR/BOOTX64.EFI"

rm -f main.o func.o
cd "$ROOT"

echo "Build complete successfully!"