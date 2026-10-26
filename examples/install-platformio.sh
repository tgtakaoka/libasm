#!/bin/bash

export BINDIR=${HOME}/.local/bin
which platformio || {
    curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
    python3 get-platformio.py
    hash -r
    }
