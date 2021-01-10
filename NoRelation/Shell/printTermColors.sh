#!/bin/bash

for i in {0..31} ; do for j in {0..7} ; do (( x=$i*8+$j )) ; printf "\e[38;5;%dm████ %03d ████  \e[m" $x $x ; done ; printf "\n" ; done
