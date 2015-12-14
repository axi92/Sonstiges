#!/bin/bash
echo `date +%Y-%m-%d-%H%:%M:%S` >> cpu.log
cat /proc/cpuinfo | grep "cpu MHz" >> cpu.log
echo ---------------------------- >> cpu.log

# crontab line for every 5 minutes
# */5 * * * * /path/to/script.sh