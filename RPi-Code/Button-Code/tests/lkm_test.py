#!/usr/bin/env python3

"""
Name: lkm_test.py
Desc: a script to test python interaction with the button LKM
Auth: Isaac Basque-Rice
Date: 15/01/2023
"""

import subprocess

# Get the last line of the kernel log
result = subprocess.run(['tail', '-1', '/var/log/kern.log'], stdout=subprocess.PIPE)

# Extract the output from the subprocess call
last_log_line = result.stdout.decode().strip()

# Check if the last log line contains "Button press detected"
if "Button press detected" in last_log_line:
    print("Hello World")

