#! /bin/sh

#set -x
set -e

# download syntax files
ROOT=https://kate-editor.org/syntax/data/
files=`wget $ROOT -q -O - | grep xml | awk '{print $5'} | cut -d\" -f 2`

for f in $files; do
    printf  "Downloading $f"
    wget $ROOT/$f -q --no-clobber &
    printf ".\n"
done

# download themes
ROOT=https://kate-editor.org/syntax/data/themes/
files=`wget $ROOT -q -O - | grep theme | awk '{print $6'} | cut -d\" -f 2`

for f in $files; do
    printf  "Downloading $f"
    wget $ROOT/$f -q --no-clobber
    printf ".\n"
done
mv *.theme ../colors/
