#!/bin/bash

# creates combined header file which can be used to include the whole engine
# into projects using this..

out_file="Fungine.hpp"

# Get script's dir
root_dir=$(dirname "$0")
cd $root_dir
root_dir=$(pwd)

cd $root_dir/src
src_dir=$(pwd)

echo "Building combined header file to $src_dir"

# Check does the "combined header file" already exist -> if it does delete it
if [ -f $out_file ]
then
    echo "Existing combined header found. Deleting..."
    rm $out_file
fi

# Create the combined header
touch $out_file
echo "#pragma once" >> $out_file
echo '' >> $out_file
find $src_dir -type f -print0 | while read -d $'\0' file; do
    if [[ $file == *.h ]]
    then
        echo "#include \"$(realpath -s --relative-to="$src_dir" "$file")\"" >> $out_file
    fi
done
