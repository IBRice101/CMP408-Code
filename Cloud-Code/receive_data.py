#!/usr/bin/env python3

"""
Name: receive_data.py
Desc: an MQTT listener for the JSON object sent from the Raspberry Pi
Auth: Isaac Basque-Rice
Date: 07/01/2023
"""

import json
from decimal import Decimal
import paho.mqtt.client as mqtt
import boto3

# TODO: clean this up and push it to a real environment 

broker = '3.82.195.208'

# Create an MQTT client
client = mqtt.Client()

# Set up a callback function to be called when a message is received
def on_message(userdata, message):
    '''Callback function to be called when a message is received on the MQTT topic.'''
    # Parse the JSON data from the message
    data = json.loads(message.payload.decode("ascii", "ignore"))
    dynamodb = boto3.resource("dynamodb")
    table = dynamodb.Table("speedtest")
    data["Upload"] = Decimal(data["Upload"])
    data["Download"] = Decimal(data["Download"])
    table.put_item(Item=data)
    # TODO: write code to update the website here


# Connect to MQTT broker (Raspberry Pi)
client.connect(broker, 1883)

# Set the callback function to be called when a message is received
client.on_message = on_message

# Subscribe to the same topic that the Raspberry Pi is publishing to
client.subscribe('speedtest-data')

# Start the MQTT client loop to receive messages
client.loop_forever()
