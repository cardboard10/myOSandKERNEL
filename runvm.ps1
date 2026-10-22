echo "running QEMU"
cd C:\Users\lando\projs\OS\UEFI1
qemu-system-x86_64 -bios OVMF_CODE.fd -drive format=raw,file=fat:rw:esp -net none