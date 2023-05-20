# Operating-Systems
To run:
make
sudo insmod CSC1107_11_kernel.ko
gcc CSC1107_11_user.c -o CSC1107_11_user -lcrypto
sudo ./CSC1107_11_user

To remove fernel module:
sudo rmmod CSC1107_11_kernel

To clear log:
sudo dmesg -C

To view log:
sudo dmesg
