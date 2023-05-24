#! /usr/bin/env bash
#
# Run this inside the directory of the modules
# This script will compile the kernel module, remove the old module, insert the new module, compile the user file and run it 5 times

make
sudo rmmod CSC1107_11_kernel 
sudo insmod CSC1107_11_kernel.ko 
sudo dmesg -C
gcc -o CSC1107_11_user CSC1107_11_user.c -lcrypto

# Run the user file for some time
for i in {1..5}
do
	sudo ./CSC1107_11_user
done

sudo dmesg | less
