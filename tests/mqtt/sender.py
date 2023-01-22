import paho.mqtt.client as mqtt
import json

# MQTT settings
broker_address = "3.91.200.59"
username = "user"
password = "password"
topic = "testdata"

# Create MQTT client
client = mqtt.Client()

# Set username and password as auth tokens
client.username_pw_set(username, password)

# Connect to broker
client.connect(broker_address)

# Read the JSON file
with open("data.json", "r") as f:
    data = json.load(f)

# Convert the JSON data to a string
data_str = json.dumps(data)

# Send the JSON data to the server
client.publish(topic, data_str)

# Disconnect from broker
client.disconnect()
