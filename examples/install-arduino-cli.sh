#!/bin/bash

export BINDIR=${HOME}/.local/bin
echo "rm -f ${BINDIR}/arduino-cli"
rm -f ${BINDIR}/arduino-cli

which arduino-cli || {
    curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
    hash -r
    }
test -f ${HOME}/.arduino15/arduino-cli.yaml || arduino-cli config init

arduino-cli config add board_manager.additional_urls https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json
# drop the descartes.net mirror left over from when drazzy.com was down
arduino-cli config remove board_manager.additional_urls https://descartes.net/package_drazzy.com_index.json
arduino-cli config add board_manager.additional_urls https://drazzy.com/package_drazzy.com_index.json
arduino-cli config add board_manager.additional_urls https://www.pjrc.com/teensy/package_teensy_index.json
arduino-cli core update-index

arduino-cli core install MightyCore:avr
arduino-cli core install DxCore:megaavr
arduino-cli core install teensy:avr

arduino-cli lib install libcli@1.4.2
