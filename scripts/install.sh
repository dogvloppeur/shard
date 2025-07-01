#!/bin/bash

set -e

if [ "$(id -u)" -ne 0 ]; then
    echo "This script must be run as root."
    exit 1
fi

BIN="../shard"
INSTALL_DIR="/usr/local/bin"

if [ ! -f "$BIN" ]; then
    echo "Executable not found. Please run 'make' first."
    exit 1
fi

install -m 0755 "$BIN" "$INSTALL_DIR/shard"