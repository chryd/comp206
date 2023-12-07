#Author: Christine Yang-Dai (ID: 260990057)
#!/bin/bash

#In the case where there is a wrong number of arguments
if [[ $# -ne 2  ]] 
then
	echo "Usage ./csv2html.sh <input.csv> <output.htlm>"
	exit 1
fi

#setting the variable
inputf=$1
outputf=$2

#for when the directory is not valid
if [[ ! -f $inputf ]]
then
	echo "Error! $inputf is not a valid directory"
	exit 1
fi

echo "<TABLE>" > $outputf
sed -e 's/^/<TR><TD>/g' -e 's;$;</TD></TR>;g' -e 's;,;</TD><TD>;g' $inputf >> $outputf 
echo "</TABLE>" >> $outputf
