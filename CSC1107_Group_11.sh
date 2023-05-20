#! /usr/bin/env bash
#
# To execute this script run the following command:
# sudo ./CSC1107_Group_11.sh
#
# NOTE: Script needs to be run as root

# Variables
USER=$(whoami)
CURRENT_DIR=$(pwd)
HOME_DIR="/home/$USER"
SHELL="/usr/bin/bash"
NEW_USER_NAME="CSC1107_GROUP_11"
FOLDER_GROUP_11="$HOME_DIR/Folder_Group_11/"

LKM_FOLDER="$HOME_DIR/linux/CSC1107_assignment/"
LKM_FILE="CSC1107_11_kernel.ko"

USER_FILE="CSC1107_11_user.c"
USER_FILE_EXECUTABLE="CSC1107_11_user"

# Check for sudo permissions
if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

# Part 1
echo "Current User: $USER"

# Part 2
echo "Current Directory: $CURRENT_DIR"

# Part 3 & 4: Create new User (Uncomment to run) and use bash as default shell
# useradd -m -s $SHELL newuser

# Part 4: Display line containing new user

# grep $NEW_USER_NAME /etc/passwd

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
        # Might need to change permissions
    fi
else
    mkdir $FOLDER_GROUP_11
    # Might need to change permissions
    echo "Folder created"
fi

# Part 7: Change Directory to Folder_Group_11 (Not sure why this is needed)
cd $FOLDER_GROUP_11

# Part 8: Copy LKM to Folder_Group_11
# This should copy:
# 1. CSC1107_Group_11_kernel.c
# 2. CSC1107_Group_11_user.c
# 3. makefile
# 4. CSC1107_Group_11_.sh (This file)

# cp -r $LKM_FOLDER $FOLDER_GROUP_11 #CAUTION: This will copy the folder and all its contents

# Part 9: Compile LKM with make
make -C $LKM_FOLDER

# Part 10: List all files in Folder_Group_11
ls -a $FOLDER_GROUP_11

# Part 11: Insert LKM
insmod $LKM_FOLDER/CSC1107_Group_11_kernel.ko

# Part 12: Check if LKM is inserted with lsmod and grep
lsmod | grep $($LKM_FILE | cut -d '.' -f 1) # Get LKM name without extension

# Part 13: Use modinfo to display information about LKM
modinfo $LKM_FOLDER/CSC1107_Group_11_kernel.ko

# Part 14: use dmesg to display kernel messages
dmesg | tail -n 5

# Part 15: Compile user program
gcc $LKM_FOLDER/$USER_FILE -o $LKM_FOLDER/$USER_FILE_EXECUTABLE

# Part 16: Run user program
$LKM_FOLDER/$USER_FILE_EXECUTABLE

# Part 17 and 18 not needed here (to observe only)

# Part 19: Display last 12 lines of syslog file
tail -n 12 /var/log/syslog

# Part 20: Remove LKM
rmmod $(LKM_FILE | cut -d '.' -f 1) # Get LKM name without extension

# Part 21: Check if LKM is removed dmesg 
dmesg | tail -3

# Part 22: Delete user
userdel -r $NEW_USER_NAME

# Part 23: Use grep to check if user still exists
grep $NEW_USER_NAME /etc/passwd

# Part 24: Display current working directory
pwd

# Part 25: List all files in current Directory
ls -a $CURRENT_DIR

# Print msg on screen
#
echo "The bash shell script of CSC1107_Group_11 has finished all tasks, and stop here."
