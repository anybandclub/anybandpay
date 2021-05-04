#!/bin/sh
i=0
while [ $i -lt 1000 ] 
do
./abp-cli sendfrom "main" yZ8Zcs9CSdM9f5M5XhzSZHLeLhdtUhjnhV 50  > $print.txt
cat $print.txt
i=$(($i+1))
sleep 1
done
