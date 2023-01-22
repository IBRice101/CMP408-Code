import paho.mqtt.client as mqtt

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
    print("Received message: ", message.payload)

# Set callback function
client.on_message = on_message

# Keep script running to receive messages
client.loop_forever()

