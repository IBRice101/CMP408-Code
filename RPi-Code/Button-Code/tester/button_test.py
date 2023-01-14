#!/usr/bin/env python3

"""
Name: button_test.py
Desc: a script to test if the GPIO button is working
Source: https://raspberrypihq.com/use-a-push-button-with-raspberry-pi-gpio/
Auth: Isaac Basque-Rice
Date: 14/01/2023
"""

import RPi.GPIO as GPIO # Import Raspberry Pi GPIO library

def button_callback(channel):
    print("Button was pushed!")

GPIO.setwarnings(False) # Ignore warning for now
GPIO.setmode(GPIO.BOARD) # Use physical pin numbering
GPIO.setup(10, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) # Set pin 10 to be an input pin and set initial value to be pulled low (off)

GPIO.add_event_detect(10,GPIO.RISING,callback=button_callback) # Setup event on pin 10 rising edge

message = input("Press enter to quit\n\n") # Run until someone presses enter

GPIO.cleanup() # Clean up

