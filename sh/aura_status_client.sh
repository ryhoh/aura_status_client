#!/bin/bash

NAME='client_name'

while true
do
    # if having GPGPUs by nvidia...
    if type nvidia-smi >/dev/null 2>&1
    then
    smi=`nvidia-smi`
    else
    smi=''
    fi
    
    # send
    curl -X POST -d "name=$NAME&smi=$smi" https://aurastatusboard.herokuapp.com/api/heartbeat
    
    # Hour [0, 6]: take deep sleep until 6.
    hour=`date "+%H"`
    if [ $hour -lt 6 ]
    then
        sleep $(( (6 - hour) * 3600 ))
    else  # 15 min sleep
        sleep 900
    fi
done
