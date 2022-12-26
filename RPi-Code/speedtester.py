#!/usr/bin/env python3

"""
Name: speedtest.py
Desc: a program to test the speed of a computer network
Auth: Isaac Basque-Rice
Date: 24/12/2022
"""

import time
import json
import os

import speedtest

def send_to_json(now, ssid, down_speed, up_speed, ping, runtime):
    """ sends data from test function to a JSON object """
    ssid = str.strip(ssid) # removes \n

    # full list of information to send to JSON object
    dictionary = {
        "Start time": now,
        "Network name": ssid,
        "Down speed": down_speed,
        "Up speed": up_speed,
        "Ping": ping,
        "Run time": runtime
    }

    filename = "speed.json"

    # dumps formats it properly (dump mkes it a one-liner)
    json_object = json.dumps(dictionary, indent = 4)

    # append instead of write so it doesn't keep getting overwritten
    with open(filename, "a", encoding='ascii') as outfile:
        outfile.write(json_object + ",\n")

def test():
    """ Performs the speedtest and prints download, upload speeds and ping """
    speed = speedtest.Speedtest()
    
    # apparently I need this for ping? idk
    servernames = []
    speed.get_servers(servernames)

    down_speed = speed.download()
    up_speed = speed.upload()
    ping = speed.results.ping

    # will eventually find a neat way to convert bits to megabits or gigabits
    print("Download speed: " + str(int(down_speed)) + " b/s")
    print("Upload speed: " + str(int(up_speed)) + " b/s")
    print("Ping: " + str(int(ping)) + " ms")

    # casting to int to make the numbers less unwieldy lol
    return int(down_speed), int(up_speed), int(ping)

def main():
    """ The main function """
    start = time.time()
    now = time.ctime(start) # neater format (no epoch for human cosumption)
    ssid = os.popen("iwgetid -r").read() # get SSID from OS shell

    print("Beginning speedtest of " + ssid + str(now))

    # run the function and grab all the values
    down_speed, up_speed, ping = test()

    end = time.time()

    print("Speedtest completed in " + str(int(end-start)) + " seconds")

    print("Writing to JSON object...")

    # send that to the JSON object
    send_to_json(now, ssid, down_speed, up_speed, ping, int(end-start))

    print("Writing complete")


if __name__ == "__main__":
    main()
