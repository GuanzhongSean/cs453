URL="http://ugster72a.student.cs.uwaterloo.ca/a1/"
PWGENC="pwgen.c"
SHELLCODEH="shellcode.h"
sudo apt-get update
yes | sudo DEBIAN_FRONTEND=noninteractive apt-get -yq install gcc libssl-dev gdb zsh expect python3 build-essential gcc-12
sudo ln -sf /bin/zsh /bin/sh
sudo sysctl -w kernel.randomize_va_space=0
sudo sysctl -w fs.protected_symlinks=0
sudo sysctl -w fs.protected_regular=0
wget "${URL}${PWGENC}"
wget "${URL}${SHELLCODEH}"
gcc -z execstack -fno-stack-protector -g -o pwgen pwgen.c -lcrypt -lcrypto -lssl
sudo chown root:root pwgen
sudo chmod 4755 pwgen
sudo mv pwgen /usr/local/bin
