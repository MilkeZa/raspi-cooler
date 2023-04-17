#!/bin/bash

# File   : trigger-fan.sh
# Author : Zachary Milke
# Description :
#   Read the CPU temperature, strip the output such that all that remains is a
#   string representation of the approximate temperature, in degress Celsius.
#   Then, pass the float to the trigger_fan executable to set the current state
#   of the cooling fan.
#
# Created on : 13-APR-2023
# Updated on : 17-APR-2023

# Step 1: Fetch the approximate CPU temperature and strip all but two leading digits,
# demical place, and trailing digit. We can do it using the cut command as we
# know what the output format of the vcgencmd command is, e.g., 'temp=40.5'C'.
cpu_temp=$(vcgencmd measure_temp | cut -c 6-9)

# Step 2: Call the trigger_fan executable passing the cpu_temp as an argument.
sudo ./trigger_fan $cpu_temp
