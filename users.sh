#!/bin/bash

if [ $# -ne 3 ]; then
  echo "Usage: $0 <filename> <group_name> <-a|-r>"
  exit 1
fi

filename=$1
group_name=$2
operation_flag=$3

# check if file exists
if [ ! -f "$filename" ]; then
  echo "Error: filename does not exist."
  exit 1
fi

add_users() {
  while IFS=' ' read -r user_id password_hash; do
    # skip empty or blank lines
    if [ -z "$user_id" ]; then
      continue
    fi
    # add the user and set password
    #sudo adduser -m -G "$group_name" -p "$password_hash" "$user_id"
    sudo useradd -m "$user_id" && echo "$user_id:$password_hash" | sudo chpasswd
    echo "User '$user_id' added."
  done < "$filename"
}

remove_users() {
  while IFS=' ' read -r user_id _; do
    # skip empty or blank lines
    if [ -z "$user_id" ]; then
      continue
    fi
    # add the user and set password
    sudo userdel -r "$user_id"
    echo "User '$user_id' deleted."
  done < "$filename"
}

# check if group exists, if not, create it
if ! getent group "$group_name" > /dev/null; then
  sudo addgroup --force-badname "$group_name"
  echo "Group '$group_name' created."
else
  echo "Group '$group_name' already exists!"
fi

# run operation
case $operation_flag in
  -a)
      add_users
      ;;
  -r)
      remove_users
      sudo groupdel "$group_name"
      echo "Group '$group_name' deleted."
      ;;
  *)
      echo "Invalid operation. Use -a to add or -r to remove"
      exit 1
      ;;
esac

# Author: Shazeb Suhail
# Date: Sun.Dec.3.2023
