#!/bin/bash

NAME='client_name'

while true
do
   curl -X POST -d "name=$NAME" https://aurastatusboard.herokuapp.com/api/heartbeat
    
    # Hour [0, 6]: take deep sleep until 6.
    hour=`date "+%H"`
    if [ $hour -lt 6 ]
    then
        sleep $(( (6 - hour) * 3600 ))
    else  # 15 min sleep
        sleep 900
    fi
done
