#!/bin/bash

export BINDIR=${HOME}/.local/bin
which platformio || {
    curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
    python3 get-platformio.py
    hash -r
    }
platformio pkg install -g -l libcli

echo '@@ You may want to install udev rules'
echo '@@ See https://docs.platformio.org/en/stable/core/installation/udev-rules.html'
echo '@@ Or run the following command'
echo 'curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules'

