# Raspberry Pi Network Speed Tester Project

This is my CMP408 Mini-Project, a network speed tester and associated web client.

## Hardware & Software

- Raspberry Pi Zero W (And Associated Hardware and Software)
- Breadboard
- Button
- Red & Green LED
- Wires
- Resistors
- AWS Web Server
- Flask

## Use Flow And Requirements

1. User presses button
    - Button needs to be wired in properly
    - Needs an LKM to handle the button press
    - Need to figure out how to get the button press to trigger a script running
2. Speed test begins
    - Need to find or create a script to work with this (most likely in Python)
3. Red LED turns on (Indicates test is happening)
    - Requires LKM, although I believe one has already been written, need to look into this
4. Results are collected (Ideally in JSON format)
    - Hopefully there is some library that allows for this kind of output
5. Results are sent to web server
    - This will probably be fairly easy although I'm unsure at this stage how to do this 
6. Server parses the results and displays them in some format
    - Likely also fairly easy, Flask is python-based so there'll be a library to do this
7. Green LED turns on
    - See 3

## Directory Structure

- RPI-Code: all code designed to run on the raspberry pi, including the speedtest application itself, Linux kernel modules, and other auxiliary programs required for functioning of the system
- Cloud-Code: Code to run on the AWS machine, pretty much just the web server.
