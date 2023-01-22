import paho.mqtt.client as mqtt
import time
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

# Subscribe to topic
client.subscribe(topic)

# Define callback function for incoming messages
def on_message(client, userdata, message):
    # convert the received message (string) to json format
    data = json.loads(message.payload)
    # save the json data to a file named "data.json"
    with open("data.json", "w") as f:
        json.dump(data, f)
    print("Data saved to disk")

# Set callback function
client.on_message = on_message

client.loop_forever()
