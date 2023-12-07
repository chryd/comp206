#!/bin/bash

#check if the number of arguments is appropriate
if [[ $# -ne 2 ]]
then
	echo "Usage: ./backup.sh backupdirname filetobackup"
	exit 1
else
	#setup some variables to be clearer
	backupdirname=$1
	tobackupdir=$2
	file=$(basename $2)
	today=$(date +"%Y%m%d")
	new_file=$file.$today
fi

#for other cases:

#!!!!MISSING did not successfully check for an empty source directory and incorrect return code !!!!!!!!!!!!!!!!!

if [[ ! -d $backupdirname ]] #when the directory is not valid
then
	echo "Error!! $backupdirname is not a valid directory"
	exit 1
elif [[ ! -f $tobackupdir ]] #if the file to backup does not exist
then
	echo "Error $file is not a valid file"
	exit 1
elif [[ -f $backupdirname/$new_file ]] #if the backup file for today already exist
then
	echo "Backup file already exists for $today"
	exit 1
else
	#else, the file is copied to the new file in the backup directory
	cp $tobackupdir $backupdirname/$new_file
	exit 0
fi
