#!/usr/bin/env bash
# File: install-OSX.command

path=~/Documents/ROLI/LittleFoot/

cd -- "$(dirname "$0")"  # switch to directory containing the script
mkdir -p "$path"         # create directory if it doesn't exist
cp    *.littlefoot      "$path"
cp -R *.littlefootModes "$path" 2>/dev/null  # 2>/dev/null redirects output nowhere
                                             # so no error is printed in absence of modes
