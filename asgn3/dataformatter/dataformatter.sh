#Author: Christine Yang-Dai (ID: 260990057)
#!/bin/bash

#Q1.
#In the case where there is a wrong number of arguments
if [[ $# -ne 1  ]] 
then
	echo "Usage ./dataformatter.sh sensorlogdir"
	exit 1
fi

#setting the variable
sensorlogdir=$1
errorOutput=""

#Q2.
#for when the directory is not valid
if [[ ! -d $sensorlogdir ]]
then
	echo "Error! $sensorlogdir is not a valid directory" > /dev/stderr #redirecting to stderr
	exit 1
fi

#Q3.
for file in $(find $sensorlogdir -name 'sensordata*.log') #for file with corresponding name
do
	#Q4.
	echo "Processing sensor data set for <`pwd`/$file>"	#retrieving the full pathname of the file
	echo "Year,Month,Hour,Sensor1,Sensor2,Sensor3,Sensor4,Sensor5"
	#in order, replacing "  " by " ", ":" by " ", "-" by " ", "  " by " -" and printing only lines with readout
	#for a more detailed explaination of my thinking process, read README.txt submitted with my assignment
	sed -n -e 's/  / /g'  -e 's/:/ /g' -e 's/-/ /g' -e 's/  / -/g' -e '/readout/P' $file | awk '
	BEGIN {OFS=","} #set the output field as ","
        #for each line	
	{
		#for each position containing the temp for each sensor, 
		#if there is an error, replace by the value of previous non erroneous sensor

		if($9 == "ERROR" )
			{gsub ($9, s1, $0)}
		
		if($10 == "ERROR" )
			{gsub ($10, s2, $0)}
		
		if($11 == "ERROR" )
			{gsub ($11, s3, $0)}
		
		if($12 == "ERROR" )
			{gsub ($12, s4, $0)}
			
		if($13 == "ERROR" )
			{gsub ($13, s5, $0)}
	}
	#print the info and set the temporary sensor value to the printed value
	{print $1, $2, $4, $9, $10, $11, $12, $13;  s1=$9; s2=$10; s3=$11; s4=$12; s5=$13}'
	echo "===================================="

	#Q5.
	echo "Readout statistics"
	echo "Year,Month,Hour,MaxTemp,MaxSensor,MinTemp,MinSensor"
	#same thinking process than for the previous question
	sed -n -e 's/  / /g'  -e 's/:/ /g' -e 's/-/ /g' -e 's/  / -/g' -e '/readout/P' $file | awk '
	BEGIN {OFS=","}
	#set some impossible numbers for the min and max
	{ MaxTemp=-101; MinTemp=101} 
	{
		#check the value of each sensor to see if its the min. If so, change the Min and the MinSensor.
		if ( $9 < MinTemp && $9 != "ERROR") { MinTemp = $9; MinSensor="Sensor1" }
		if ( $10 < MinTemp && $10 != "ERROR" ) { MinTemp = $10; MinSensor="Sensor2" }
		if ( $11 < MinTemp && $11 != "ERROR" ) { MinTemp = $11; MinSensor="Sensor3" }
		if ( $12 < MinTemp && $12 != "ERROR" ) { MinTemp = $12; MinSensor="Sensor4" }
		if ( $13 < MinTemp && $13 != "ERROR" ) { MinTemp = $13; MinSensor="Sensor5" }

		#same thing with max
		if ( $9 > MaxTemp && $9 != "ERROR" ) { MaxTemp = $9; MaxSensor="Sensor1" }
		if ( $10 > MaxTemp && $10 != "ERROR" ) { MaxTemp = $10; MaxSensor="Sensor2" }
		if ( $11 > MaxTemp && $11 != "ERROR" ) { MaxTemp = $11; MaxSensor="Sensor3" }
		if ( $12 > MaxTemp && $12 != "ERROR" ) { MaxTemp = $12; MaxSensor="Sensor4" }
		if ( $13 > MaxTemp && $13 != "ERROR" ) { MaxTemp = $13; MaxSensor="Sensor5" }
	}
	{ print $1, $2, $4, MaxTemp, MaxSensor, MinTemp, MinSensor }'
	echo "===================================="

	#Q6.
	#to not get mixed, errorForFile is the error stats for each file
	errorForFile=$(sed -n  -e 's/:/ /g' -e 's/-/ /g' -e '/rebooting/p' $file | awk '
	BEGIN {e1=0; e2=0; e3=0; e4=0; e5=0}
	{
		#if an error for a sensor is registered, add to the error counter for the corresponding sensor
		if ( $0 ~ /sensor 1/ ) { e1=e1+1 }
		if ( $0 ~ /sensor 2/ ) { e2=e2+1 }
		if ( $0 ~ /sensor 3/ ) { e3=e3+1 }
		if ( $0 ~ /sensor 4/ ) { e4=e4+1 }
		if ( $0 ~ /sensor 5/ ) { e5=e5+1 }
	}
	END { print $1, $2, $3, e1, e2, e3, e4, e5, e1+e2+e3+e4+e5 }')
	#the error stats for each file is appended to all the error stats that are going to be output
	errorOutput+="${errorForFile}\n"
done

#removing the last \n of the ouput for error stats
errorOutput=${errorOutput::-2}
	
echo "Sensor error statistics"
echo "Year,Month,Day,Sensor1,Sensor2,Sensor3,Sensor4,Sensor5,Total"
#sort error Output by decreasing number for the total, the day, the month and the year in this order exactly
#using sed to change the field seperator to ','
echo -e $errorOutput | sort -nr -k9,9 -k3,3 -k2,2 -k1,1 | sed -e 's/ /,/g'
echo  "===================================="
