#!/bin/bash

# By Mateusz Midor, III 2016
# This script commits subsequent ChapterN folders to git repo.
# The commit messages are read from "Index.txt"

function processFolderComment
{
	IFS=' - ' read -r folder_name comment <<< "$line" 
	echo "Folder: $folder_name"
	echo "Comment: $comment"
	echo

	git add "$folder_name"
	git commit -m "$comment"
}

while read line
do
	processFolderComment "$line"
done < "Index.txt" 
