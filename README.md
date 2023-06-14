# Operating-Systems

## Installation
Ensure that the modules are in `/linux/CSC1107_assignment/` **Directory**
- `CSC1107_11_kernel.c`
- `CSC1107_11_user.c`
- `Makefile`
- `CSC1107_11.sh`

Add the following line to `/etc/xdg/lxsession/LXDE-pi/autostart` to run on startup
```bash
@lxterminal -e sudo /linux/CSC1107_assignment/CSC1107_Group_11.sh
```
All modules will be compiled and executed.

## To run the modules individually
```bash
make
sudo insmod CSC1107_11_kernel.ko
gcc CSC1107_11_user.c -o CSC1107_11_user -lcrypto
sudo ./CSC1107_11_user<br>
```

## Kernel Modules
To remove kernel module
```bash
sudo rmmod CSC1107_11_kernel
```
To clear log:
```bash
sudo dmesg -C
```

To view log:
```bash
sudo dmesg
```
