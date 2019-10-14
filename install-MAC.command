#!/usr/bin/env bash
# File: install-OSX.command

path=~/Documents/ROLI/LittleFoot/

cd -- "$(dirname "$0")"  # switch to directory containing the script
mkdir -p "$path"         # create directory if it doesn't exist

echo ""
echo ""

# Check if there are apps to install:
if [[ ! $(find . -maxdepth 1 -iname '*.littlefoot' -type f) ]]
then
    echo "ERROR: no \`.littlefoot\` files found!"
    echo ""
    echo "Place \`install-OSX.command\` in the same folder as \`.littlefoot\` files you want to install and try again!"
    echo ""
    echo ""
    echo "> You may close this window now <"
    echo ""
    echo ""
    exit 1
fi

echo "Installation Progress"
echo ""

find . -maxdepth 1 -iname '*.littlefoot' -type f -print0 | while IFS= read -rd $'\0' file; do
    # 2>/dev/null supresses console output
    cp -f  "$file" "$path" 2>/dev/null && echo "INSTALLED: [App]   $(basename "$file")" || echo "FAILED:    [App]   $(basename "$file")"
done

find . -maxdepth 1 -iname '*.littlefootModes' -type d -print0 | while IFS= read -rd $'\0' file; do
    # 2>/dev/null supresses console output
    cp -fR "$file" "$path" 2>/dev/null && echo "INSTALLED: [Modes] $(basename "$file")" || echo "FAILED:    [Modes] $(basename "$file")"
done

echo ""
echo ""
echo "> You may close this window now <"
echo ""
echo ""
