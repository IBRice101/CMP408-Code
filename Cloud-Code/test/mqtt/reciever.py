import paho.mqtt.client as mqtt

# MQTT settings
broker_address = "35.173.187.104"
topic = "testdata"

# Create MQTT client
client = mqtt.Client()

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

