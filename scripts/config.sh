#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
    echo "This program must be run as root"
fi

if ! dpkg -l gcc 2>/dev/null | grep -q '^ii'; then
    echo "Installing GCC..."
    apt install gcc
fi