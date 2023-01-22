from flask import Flask, render_template
import json
import paho.mqtt.client as mqtt

app = Flask(__name__)

# Create an MQTT client
client = mqtt.Client()

# Define the MQTT broker
broker = '3.82.195.208'

def replace_json(data):
    with open('data.json', 'w') as f:
        json.dump(data, f)

# Set up a callback function to be called when a message is received
def on_message(message):
    '''Callback function to be called when a message is received on the MQTT topic.'''
    # Parse the JSON data from the message
    data = json.loads(message.payload.decode("ascii", "ignore"))
    # Use the data to replace the already existing json file
    replace_json(data)

@app.route('/')
def index():  # put application's code here
    return render_template("index.html")

@app.route('/data')
def data():
    with open('data.json', 'r') as file:
        data = file.read()
        file.close()
    return data

if __name__ == '__main__':
    # Connect to MQTT broker (Raspberry Pi)
    client.connect(broker, 1883)

    # Set the callback function to be called when a message is received
    client.on_message = on_message

    # Subscribe to the same topic that the Raspberry Pi is publishing to
    client.subscribe('speedtest-data')

    # Start the MQTT client loop to receive messages
    client.loop_start()
    app.run()
    client.loop_stop()