#!/bin/bash
cat ~/.surf/history | sort -ru | dmenu -fn "Source Code Pro for Powerline-10" \
	-nb "#FDF6E3" -nf "#657B83" -sb "#93A1A1" -sf "#EEE8D5" 
