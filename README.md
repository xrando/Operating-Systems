# Operating-Systems
To run:<br>
make<br>
sudo insmod CSC1107_11_kernel.ko<br>
gcc CSC1107_11_user.c -o CSC1107_11_user -lcrypto<br>
sudo ./CSC1107_11_user<br>
<br>
To remove kernel module:<br>
sudo rmmod CSC1107_11_kernel<br>
<br>
To clear log:<br>
sudo dmesg -C<br>
<br>
To view log:<br>
sudo dmesg<br>
