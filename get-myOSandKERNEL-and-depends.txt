# download myOSandKERNEL and install scoop and use scoop to install: gcc and nasm for compiling the source code, qemu for simulating it, git for grabbing the source code
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression
scoop install qemu
scoop install gcc
scoop install nasm
scoop install git
mkdir git
cd git
git clone https://github.com/cardboard10/myOSandKERNEL