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

# Send test data to server
test_data = "This is a test message"
client.publish(topic, test_data)

# Disconnect from broker
client.disconnect()

