$Root = $PSScriptRoot
$SrcDir = "$Root\src"

Write-Host "building program"
Set-Location $SrcDir

Write-Host " 1. Clean up old build files"
Remove-Item main.o, BOOTX64.EFI -ErrorAction SilentlyContinue

Write-Host " 2. Compile main.c"
Write-Host "compiling----------------------------------------------------------------------------------------------------------------------------"
Write-Host "asm..."
nasm -f win64 func.asm -o func.o

Write-Host "C..."
gcc -ffreestanding -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -I"$SrcDir\efi-headers\" -I"$SrcDir\myheaders\" -c main.c -o main.o
Write-Host "finished"
Write-Host "----------------------------------------------------------------------------------------------------------------------------"

Write-Host " 3. Link it as a UEFI application with explicit entry point"
cmd /c "gcc -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -Wl,-e,efi_main -o BOOTX64.EFI main.o func.o -lgcc" 

Write-Host " 4. Move it into your ESP boot folder"
$DestDir = "$Root\esp\EFI\BOOT"
if (!(Test-Path $DestDir)) {
    New-Item -ItemType Directory -Force -Path $DestDir | Out-Null
}
Move-Item -Path "BOOTX64.EFI" -Destination "$DestDir\BOOTX64.EFI" -Force

Remove-Item main.o -ErrorAction SilentlyContinue
Remove-Item func.o -ErrorAction SilentlyContinue
Set-Location $Root