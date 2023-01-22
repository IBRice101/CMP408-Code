import paho.mqtt.client as mqtt

# MQTT settings
broker_address = "35.173.187.104"
topic = "testdata"

# Create MQTT client
client = mqtt.Client()

# Connect to broker
client.connect(broker_address)

# Send test data to server
test_data = "This is a test message"
client.publish(topic, test_data)

# Disconnect from broker
client.disconnect()

