#! /bin/sh

ROOT=https://kate-editor.org/syntax/data/
files=`wget $ROOT -q -O - | grep xml | awk '{print $5'} | cut -d\" -f 2`

for f in $files; do
    printf  "Downloading $f"
    wget $ROOT/$f -q --no-clobber &
    printf ".\n"
done

