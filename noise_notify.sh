#!/bin/bash

#This is a script which can detect the adc number and give a feedback to display.

while true

do

     noise_level=`/bin/adc`

     if [ ${noise_level}  > 5000 ]; then

      figlet  "Be  quite  please!"

      sleep 5

     fi

     sleep 2

done
