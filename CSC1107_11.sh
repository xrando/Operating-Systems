#! /usr/bin/env bash
#
# To execute this script run the following command:
# sudo ./CSC1107_Group_11.sh
#
# NOTE: Script needs to be run as root
#
# Add this line to /etc/xdg/lxsession/LXDE-pi/autostart to run on startup
# @lxterminal -e sudo /linux/CSC1107_assignment/CSC1107_Group_11.sh

RUN_ON_BOOT=1

if [[ $RUN_ON_BOOT -eq 0 ]]; then
    echo "RUN_ON_BOOT is set to 0, exiting script"
    exit
fi

# Check a file for amount of times it has been run, increment if it has been run more than once
# if file doesn't exist, create it and set to 1
# If content is 3, or RUN_ON_BOOT is 0, exit script
FILE=$(pwd)/CSC1107_11_run_count.txt
if [ -f "$FILE" ]; then
    RUN_COUNT=$(cat $FILE)
    if [ $RUN_COUNT -eq 3 ]; then
        echo "Script has been run $RUN_COUNT times, exiting script"
        # Reset run count to 0
        echo 0 > $FILE
        exit
    else
        echo $((RUN_COUNT+1)) > $FILE
    fi
else
    echo 1 > $FILE
fi

# Variables
USER=$(whoami)
CURRENT_DIR=$(pwd)
HOME_DIR=$HOME
SHELL="/usr/bin/bash"
NEW_USER_NAME="CSC1107_11"
FOLDER_GROUP_11="$HOME_DIR/Folder_11" # YOUR HOME DIRECTORY for part 6

LKM_FOLDER="/linux/CSC1107_assignment"
LKM_FILE="CSC1107_11_kernel.ko"
LKM=$(echo $LKM_FILE | cut -d '.' -f 1)

USER_FILE="CSC1107_11_user.c"
USER_FILE_EXECUTABLE=$(echo $USER_FILE | cut -d '.' -f 1)

# Check for sudo permissions
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

# Part 1
echo
echo "Current User: $USER"

# Part 2
echo
echo "Current Directory: $CURRENT_DIR"

# Part 3 & 4: Create new User and use bash as default shell
useradd -m -s $SHELL $NEW_USER_NAME

# Part 4: Display line containing new user
echo
echo "New User: "
grep $NEW_USER_NAME /etc/passwd
echo

# Part 6: Check if folder exist and create if it doesn't
if [ -d "$FOLDER_GROUP_11" ]; then

    # Check if folder is empty
    if [ -z "$(ls -A $FOLDER_GROUP_11)" ]; then
        echo "Folder is empty"
    else
        # Clear folder
        rm -r $FOLDER_GROUP_11

        # Create Folder_Group_11
        mkdir $FOLDER_GROUP_11
    fi
else
    mkdir $FOLDER_GROUP_11
    echo
    echo "Folder created"
fi

# Part 7: Change Directory to Folder_Group_11 (Not sure why this is needed)
cd $FOLDER_GROUP_11

# Part 8: Copy LKM to Folder_Group_11
# This should copy:
# 1. CSC1107_11_kernel.c
# 2. CSC1107_11_user.c
# 3. makefile
# 4. CSC1107_11_.sh (This file)

#CAUTION: This will copy the folder and all its contents
cp -r $LKM_FOLDER/. $FOLDER_GROUP_11 

# Part 9: Compile LKM with make
make -C $LKM_FOLDER

# Part 10: List all files in Folder_Group_11
echo
echo "Files in $FOLDER_GROUP_11:"
ls -a $FOLDER_GROUP_11

# Part 11: Insert LKM
echo
echo "Inserting LKM:"
insmod $LKM_FOLDER/CSC1107_11_kernel.ko

# Part 12: Check if LKM is inserted with lsmod and grep
echo
echo "Check if LKM is inserted:"
if lsmod | grep $LKM; then
    echo "LKM inserted"
else
    echo "LKM not inserted"
fi
#lsmod | grep $LKM

# Part 13: Use modinfo to display information about LKM
echo
echo "Information about LKM:"
modinfo $LKM_FOLDER/CSC1107_11_kernel.ko

# Part 14: use dmesg to display kernel messages
echo
echo "Kernel messages:"
dmesg | tail -n 5

# Part 15: Compile user program
gcc $LKM_FOLDER/$USER_FILE -o $LKM_FOLDER/$USER_FILE_EXECUTABLE -lcrypto

# Part 16: Run user program
$LKM_FOLDER/$USER_FILE_EXECUTABLE

# Part 17 and 18 not needed here (to observe only)

# Part 19: Display last 12 lines of syslog file
echo
echo "Last 12 lines of syslog file:"
tail -n 12 /var/log/syslog

# Part 20: Remove LKM
rmmod $LKM

# Part 21: Check if LKM is removed dmesg 
echo
echo "Kernel messages to check if LKM is removed:"
dmesg | tail -3

# Part 22: Delete user
userdel -r $NEW_USER_NAME

# Part 23: Use grep to check if user still exists
echo
echo "Check if user still exists:"
if grep -q $NEW_USER_NAME /etc/passwd; then
    echo "User still exists"
else
    echo "User does not exist"
fi

# Part 24: Display current working directory
echo
echo "Current working directory:"
pwd

# Part 25: List all files in current Directory
echo
echo "Files in current directory:"
ls -a $(pwd)

# Print msg on screen

echo
echo "The bash shell script of CSC1107_11 has finished all tasks, and stop here. Press enter to exit."

read
if [ "$?" -ne 0 ]; then
    echo "Error: Press enter to exit."
    read
fi
