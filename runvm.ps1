Write-Host "running QEMU"
# Automatically set location to the folder where runvm.ps1 is saved
Set-Location $PSScriptRoot

qemu-system-x86_64 -bios OVMF_CODE.fd -drive format=raw,file=fat:rw:esp -net none
