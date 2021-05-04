#!/bin/sh
i=0
while [ $i -lt 5705 ] 
do 
	./abp-cli  sendfrom "" "FZqccYTMcUsWzezgUfe8dYYk1WiCrvLVto" 350000  > $send.txt
	cat  $send.txt
	i=$(($i+1))
	sleep 5
done
