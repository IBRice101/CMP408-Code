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
import paho.mqtt.client as mqtt

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

    filename = "data.json"

    # loads the existing data from the file
    with open(filename, "r", encoding='ascii') as infile:
        data = []
        while True:
            try:
                obj = json.load(infile)
                data.extend(obj)
            except ValueError:
                # reached the end of the file
                break

    # append the new dictionary to the list of data
    data.append(dictionary)

    # dumps formats it properly (dump makes it a one-liner)
    json_object = json.dumps(data, indent = 4)

    # write the new list of data to the file
    with open(filename, "w", encoding='ascii') as outfile:
        outfile.write(json_object)

def test():
    """ Performs the speedtest and prints download, upload speeds and ping """
    
    speed = speedtest.Speedtest()

    # apparently I need this for ping? idk
    server_names = []
    speed.get_servers(server_names)

    down_speed = speed.download()
    up_speed = speed.upload()
    ping = speed.results.ping

    # will eventually find a neat way to convert bits to megabits or gigabits
    print("Download speed: " + str(int(down_speed)) + " b/s")
    print("Upload speed: " + str(int(up_speed)) + " b/s")
    print("Ping: " + str(int(ping)) + " ms")

    # casting to int to make the numbers less unwieldy lol
    return int(down_speed), int(up_speed), int(ping)

def send_data():

    # Address of broker (aws ec2 instance)
    broker = '3.82.195.208'

    # Load the data from your JSON file
    with open('data.json', 'r', encoding='ascii') as f:
        data = json.load(f)

    # Convert the data to a JSON string
    json_data = json.dumps(data)

    # Create an MQTT client
    client = mqtt.Client()

    # Connect to your MQTT broker (could be your AWS server)
    client.connect(broker, 1883)

    # Publish the JSON data to a topic on your MQTT broker
    client.publish('speedtest-data', json_data)

    # Disconnect from the MQTT broker
    client.disconnect()


def main():
    """ The main function """

    start = time.time()
    now = time.ctime(start) # neater format (no epoch for human consumption)
    ssid = os.popen("iwgetid -r").read() # get SSID from OS shell

    print("SSID: " + ssid + "\nStart Time: " +  str(now))

    # run the function and grab all the values
    down_speed, up_speed, ping = test()

    end = time.time()

    print("Speedtest completed in " + str(int(end-start)) + " seconds")

    print("Writing to JSON object...")

    # send that to the JSON object
    send_to_json(now, ssid, down_speed, up_speed, ping, int(end-start))

    # TODO: Send to server using MQTT


    print("Speedtest complete\n")



if __name__ == "__main__":
    main()
