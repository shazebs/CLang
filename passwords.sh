#!/bin/bash

check_password_strength() {
  local password=$1

  # test minimum length
  if [[ ${#password} -lt 8 ]]; then
    echo "Failed: Password length is less than 8 characters."
    exit 1
  fi

  # test for numeric values
  if ! [[ $password =~ [0-9] ]]; then
    echo "Failed: Password does not contain a numeric character."
    exit 1
  fi

  # test for special character (@, #, $, %, *, +, =)
  if ! [[ $password =~ [@\#$%*+=] ]]; then
    echo "Failed: Password does not contain a special character"
    exit 1
  fi

  echo "Success: Password meets all criteria"
  return 0
}

if [ $# -eq 0 ]; then
  echo "Usage: $0 [password]"
  exit 1;
fi

check_password_strength "$1"

# Author: Shazeb Suhail
# Date: Sun.Dec.3.2023
