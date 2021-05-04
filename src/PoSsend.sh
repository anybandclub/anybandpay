#!/bin/sh
i=0
while [ $i -lt 2000 ] 
do 
	./abp-cli  sendfrom "" "FZoUHTLxyx5DBu1PMgCgcq2JoeVm3BQr3k" 10000  > $print.txt
	./abp-cli  sendfrom "" "Fonb1fDm2CvuvVFdBfQdNCq7gj6EvQD7wB" 10000  >> $print.txt
	./abp-cli  sendfrom "" "FfsP2boFHP99DHDsAE5RB78Zd3N3StB34P" 10000  >> $print.txt
	cat  $print.txt
	i=$(($i+1))
	sleep 50
done
