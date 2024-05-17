#!/bin/bash

# Creates include dir containing only src's header files and creates
# combined header file which can be used to include the whole engine
# into projects using this..

out_dir="include"
out_file="Fungine.hpp"


# Get script's dir
root_dir=$(dirname "$0")
cd $root_dir
root_dir=$(pwd)

echo "Building combined header file to $root_dir"

# Check if include dir exists -> if it does delete old
if [ -D "$out_dir" ]
then
    echo "Existing include dir found: $out_dir\n\tDeleting..."
    rm -rf $out_dir
fi
echo "Creating include dir: $out_dir"
mkdir $out_dir

# Copy src headers
# *Quite shit way to do this but it'll do for now..
# Copy whole src and delete cpp files from there..
cp -r src/* $out_dir/
cd $out_dir
root_dir=$(pwd)
find $root_dir -type f -print0 | while read -d $'\0' file; do
    if [[ $file == *.cpp ]]
    then
        rm $file
    fi
done


# Chech does the "combined header file" already exist -> if it does delete it
if [ -f $out_file ]
then
    echo "FILE EXISTS! Deleting old.."
    rm $out_file
fi

# Create the combined header
touch $out_file
echo "#pragma once" >> $out_file
echo '' >> $out_file
find $root_dir -type f -print0 | while read -d $'\0' file; do
    if [[ $file == *.h ]]
    then
        echo "#include \"$(realpath -s --relative-to="$root_dir" "$file")\"" >> $out_file
    fi
done
