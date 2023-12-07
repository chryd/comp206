#!/bin/bash

#if only one argument, the the source directory is set to current directory
if [[ $# -eq 1 ]]
then
	sourcedir=$(pwd)
else
	sourcedir=$2
fi

backupdirname=$1

#for all cases of error
#if the number of arguments is not appropriate
if [ $# -ne 1 ] && [ $# -ne 2 ]
then
	echo "Usage: ./ckbackups.sh backupdirname [sourcedir]"
	exit 1
#if the backup directory is wrong
elif [[ ! -d $backupdirname ]]
then
	echo "Error! $backupdirname is not a valid directory"
	exit 1
#if the source directory if wrong
elif [[ ! -d $sourcedir ]]
then
	echo "Error! $sourcedir is not a valid directory"
	exit 1
#if the number of files in the source directory is 0
elif [[ $(ls $sourcedir | wc -l) = 0 ]]
then
	echo "Error!! $sourcedir has no files"
	exit 1
else
	message=""

	#for all the files the source directory
	for file in $(ls $sourcedir)
	do
		#if the backup file already exist in the backup directory
		if [[ ! -f $backupdirname/$file.$(date +"%Y%m%d") ]]
		then 
			message=${message}"$file does not have a backup for today\n"
		fi
	done

	#the all the files are backed up
	if [[ -z $message  ]]
	then
		echo "All files in $sourcedir have backups for today in $backupdirname"
	else
		echo -e  $message
	fi
fi
