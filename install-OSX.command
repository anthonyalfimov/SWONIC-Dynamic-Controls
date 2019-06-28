#!/usr/bin/env bash
# File: install-OSX.command

function fail
{
    echo "Error: no \`.littlefoot\` files found!"
    echo ""
    echo "Place \`install-OSX.command\` in the same folder as \`.littlefoot\` files you want to install and try again!"
    echo ""
    echo ""
    exit 1
}

path=~/Documents/ROLI/LittleFoot/

cd -- "$(dirname "$0")"  # switch to directory containing the script
mkdir -p "$path"         # create directory if it doesn't exist

echo ""
echo ""
cp    *.littlefoot      "$path" 2>/dev/null || fail  # 2>/dev/null supresses console output
echo "Successfully installed:"
ls -1 | grep "littlefoot$"
cp -R *.littlefootModes "$path" 2>/dev/null && ls -1 | grep "littlefootModes$"
echo ""
echo ""
echo "> You may now close this window <"
echo ""
echo ""
