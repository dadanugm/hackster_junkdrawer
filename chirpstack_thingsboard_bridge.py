### test ###
import paho.mqtt.client as mqtt
from tb_device_mqtt import TBDeviceMqttClient, TBPublishInfo
import paho.mqtt.publish as publish
import json
import os
import base64

thingsboard_server="192.168.100.10"
mqttc = mqtt.Client("my_id")

def publish_msg (client_id, username, message):
	#telemetry = {"temperature": 41.9}
	telemetry = message
	client_thb = TBDeviceMqttClient(thingsboard_server, username=username, client_id=client_id)
	# Connect to ThingsBoard
	client_thb.connect()
	# Sending telemetry without checking the delivery status
	client_thb.send_telemetry(telemetry)
	# Sending telemetry and checking the delivery status (QoS = 1 by default)
	result = client_thb.send_telemetry(telemetry)
	# get is a blocking call that awaits delivery status
	success = result.get() == TBPublishInfo.TB_ERR_SUCCESS
	# Disconnect from ThingsBoard
	client_thb.disconnect()

def bridge_parse(payloads):
	# Parse received message and publish to thingsboard
	#lora_msg = json.loads(payloads)
	print("clientID: "+payloads["devEUI"]+", User Name: "+payloads["deviceName"]+" data: "+payloads["data"])
	#publish(hostname="mqtt.eclipseprojects.io", )
	#command = ("mosquitto_pub -d -q 1 -p 1883 -t v1/devices/me/telemetry -h "+thingsboard_server+" -i "+payloads["devEUI"]+" -u "+payloads["deviceName"]+" -m {temperature:25}")
	#os.system(command)
	node_data=base64.b64decode(payloads["data"]).decode('ascii')
	publish_msg(payloads["deviceName"],payloads["devEUI"],{"data":node_data})

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, properties):
	#print(f"Connected with result code {reason_code}")
	print("Connected to MQTT broker!")
	print("Connection flags:", flags)
        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        #client.subscribe("$SYS/#")
	client.subscribe("application/#")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
	#print(msg.topic+" "+str(msg.payload))
	lora_msg = json.loads(msg.payload)
	bridge_parse(lora_msg)

mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.connect("localhost", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
mqttc.loop_forever()
