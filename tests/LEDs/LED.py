#!/usr/bin/env python3

"""
Name: LED.py
Desc: A script to check if the LED works
Auth: The Pi Hut & Isaac Basque-Rice
Date: 23/01/2023
"""

import RPi.GPIO as GPIO
import time

gpio_pin = 9

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(gpio_pin, GPIO.OUT)

print("LED on")
GPIO.output(gpio_pin, GPIO.LOW)
time.sleep(1)

print("LED off")
GPIO.output(gpio_pin, GPIO.HIGH)
