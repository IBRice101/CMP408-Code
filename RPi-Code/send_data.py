#!/usr/bin/env python3

"""
Name: send_data.py
Desc: send data.json from Raspberry Pi to AWS server using MQTT
Auth: Isaac Basque-Rice
Date: 07/01/2023
"""

import json
import paho.mqtt.client as mqtt

# TODO: clean this up and push it to a real environment 

# Load the data from your JSON file
with open('data.json', 'r', encoding='ascii') as f:
    data = json.load(f)

# Convert the data to a JSON string
json_data = json.dumps(data)

# Create an MQTT client
client = mqtt.Client()

# Connect to your MQTT broker (could be your AWS server)
client.connect('your-broker-address', 1883)

# Publish the JSON data to a topic on your MQTT broker
client.publish('your-topic', json_data)

# Disconnect from the MQTT broker
client.disconnect()
