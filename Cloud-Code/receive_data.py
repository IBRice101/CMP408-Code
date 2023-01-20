#!/usr/bin/env python3

"""
Name: receive_data.py
Desc: an MQTT listener for the JSON object sent from the Raspberry Pi
Auth: Isaac Basque-Rice
Date: 07/01/2023
"""

import json
import paho.mqtt.client as mqtt

# TODO: clean this up and push it to a real environment 

# Create an MQTT client
client = mqtt.Client()


# Set up a callback function to be called when a message is received
def on_message(userdata, message):
    '''Callback function to be called when a message is received on the MQTT topic.'''
    # Parse the JSON data from the message
    data = json.loads(message.payload)
    # TODO: write code to update the website here


# Connect to MQTT broker (Raspberry Pi)
client.connect('your-broker-address', 1883)

# Set the callback function to be called when a message is received
client.on_message = on_message

# Subscribe to the same topic that the Raspberry Pi is publishing to
client.subscribe('your-topic')

# Start the MQTT client loop to receive messages
client.loop_forever()
