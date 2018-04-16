#!/usr/bin/env bash

# This is ugly and hacky. But it converts a single .csv file to a .txt
# suitable for igraph by removing the first line and replacing all commas with
# spaces. If a second argument is provided and is exactly "--delete-original"
# the original csv file is deleted.

csvfile="$1"
# Remove the header row from the CSV
tail -n +2 "$csvfile" > "${csvfile}.tmp"
# Replace commas with spaces
sed -i'' -e 's/,/ /g' "${csvfile}.tmp"
# Rename the temporary file to .txt
mv "${csvfile}.tmp" "$(dirname ${csvfile}.tmp)/$(basename "${csvfile}.tmp" ".csv.tmp").txt"

if [ "$2" == "--delete-original" ]; then
    rm "$csvfile"
fi
