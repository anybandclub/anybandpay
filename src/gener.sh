#!/bin/sh
i=0
while [ $i -lt 500 ] 
do 
	./abp-cli  generate 10  > $print.txt

	cat $print.txt
	i=$(($i+1))
	sleep 10
done
