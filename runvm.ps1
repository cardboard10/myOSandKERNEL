Write-Host "running QEMU"
# Automatically set location to the folder where runvm.ps1 is saved
Set-Location $PSScriptRoot
if (!(Test-Path ./OVMF_CODE.fd)) {
    curl https://qemu.weilnetz.de/test/ovmf/usr/share/OVMF/OVMF_CODE.fd -OutFile ./OVMF_CODE.fd
}
qemu-system-x86_64 -bios OVMF_CODE.fd -drive format=raw,file=fat:rw:esp -drive format=raw,file=./storage.img -net none
