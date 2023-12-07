#!/bin/bash

#check if the number of arguments is appropriate
if [[ $# -ne 2 ]]
then
	echo "Usage fixformat.sh <dirname> <opfile>"
	exit 1
else
	#setup some variable to be clearer
	dirname=$1
	opfile=$2
fi

#for other cases:
if [[ ! -d $dirname ]] #when the directory is not valid
then
	echo "Error! $dirname is not a valid directory"
	exit 1
fi

echo "User Email, Name (Original Name), Lab, Total Duration (Minutes)" > $opfile
for file in $(find $dirname -name 'lab-*.csv')
do
	lab=${file:$((${#file}-5)):1}
	sed -n -e 's,\([0-9]\{2\}\)/\([0-9]\{2\}\)/\([0-9]\{4\} [0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\} [AP]M\),,g' -e 's/,,,/,/g' -e '/@/p' $file | awk -v labNO="${lab^}" 'BEGIN {FS=",";OFS=","} ; { print $2,$1,labNO,$3 }' >> $opfile
done

