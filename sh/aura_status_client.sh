#!/bin/bash

NAME='client_name'

while true
do
   curl -X POST -d 'name=$NAME' https://aurastatusboard.herokuapp.com/api/heartbeat
   sleep 900
done
