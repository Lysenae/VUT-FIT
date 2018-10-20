// Either of these can be used and they can be used together. Just comment or uncomment
#define MQTT_OVER_WEBSOCKETS
#define USE_SSL

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <dht.h>
#include <Hash.h>
#include <MQTTWebSocketClient.h>
#include <PubSubClient.h>
#include "main.h"

/*
 * There are some issues with the implementation and porting of the libraries
 * but it seems to be working anyway. Don't be scared if you see stack or some
 * socket errors. The board might reset or just reconnect and everything should
 * work.
 *
 * EPS8266 seems to throw stack for the first connection after booting but
 * after the self soft reset everything works and connection is stable.
 */

#ifdef MQTT_OVER_WEBSOCKETS
MQTTWebSocketClient mws;
#else
#ifdef USE_SSL
WiFiClientSecure espClient;
#else
WiFiClient espClient;
#endif
#endif

PubSubClient client;
dht DHT;

// variables
char mqtt_msg[50];
uint16_t connect_cnt = 0;

unsigned char current_pc_status = 255;
unsigned char last_read_status = 255;
unsigned char last_published_status = 255;
long last_status_unstable_ms = 0;

float current_temp = -127.0;
float last_published_temp = -127.0;
long last_temp_request_ms = 0;

long last_published_ms = 0;
PubData_e data_to_publish = PUB_DATA_PC_STATUS;

///////////////////////////////////////////////////////////////////////////////
/// Init functions
///////////////////////////////////////////////////////////////////////////////
void Wifi_Connect(void) {
	Serial.println(); // before this we have crap from ESP bootloader
	Serial.print("Connecting to ");
	Serial.println(WIFI_SSID);
	// connect to Wifi network
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	while (WiFi.status() != WL_CONNECTED)	{
		delay(500);
		Serial.print(".");
	}
	Serial.println();
	Serial.print("WiFi connected! IP: ");
	Serial.println(WiFi.localIP());
}

///////////////////////////////////////////////////////////////////////////////
/// Setup and main loop
///////////////////////////////////////////////////////////////////////////////
void setup() {
	Serial.begin(115200, SERIAL_8N1);
	pinMode(GPIO_OUT_PWSW, OUTPUT);
	digitalWrite(GPIO_OUT_PWSW, OUT_STATE_INACTIVE);
	pinMode(GPIO_OUT_RTSW, OUTPUT);
	digitalWrite(GPIO_OUT_RTSW, OUT_STATE_INACTIVE);
	pinMode(GPIO_IN_STATUS, INPUT);
	delay(500);
	// connect to Wifi
	Wifi_Connect();
	#ifdef MQTT_OVER_WEBSOCKETS
	// setup WebSockets
	Serial.print("Using websockets URL:");
	Serial.print(CLOUDMQTT_SERVER);
	Serial.print(" port: ");
	Serial.println(CLOUDMQTT_PORT);
	mws.setUseSSL(SSL);
	mws.setReconnectInterval(5000);
	mws.setPath(CLOUDMQTT_PATH);
	mws.connect(CLOUDMQTT_SERVER, CLOUDMQTT_PORT);
	// setup MQTT over WebSockets w/o SSL
	delay(10000);
	client.setClient(mws);
	#else
	Serial.print("Using mqtt protocol URL:");
	Serial.print(CLOUDMQTT_SERVER);
	Serial.print(" port: ");
	Serial.println(CLOUDMQTT_PORT);
	// setup normal mqtt tcp/ssl
	client.setClient(espClient);
	#endif
	client.setServer(CLOUDMQTT_SERVER, CLOUDMQTT_PORT);
	client.setCallback(Subscription_Callback);
}

void loop() {
	// handle MQTT connection
	if (!client.connected()) {
		connect_cnt++;
		Serial.printf("conn count: %d\n", connect_cnt);
		Mqtt_Reconnect();
		last_published_ms = last_status_unstable_ms = millis(); // init time here, it takes a while to connect
	}
	client.loop();


	// handle temperature reading
	if ((millis() - last_temp_request_ms) > TEMP_REFRESH_MS) {
		// get temperature from first index (since we have only one sensor)
	 	int chk = DHT.read11(GPIO_TEMP);
		Serial.print("Temperature read: ");
		Serial.println(DHT.temperature);
		current_temp = DHT.temperature;

		// request new temperature conversion, which can take up to 750 ms (DS18B20)
		// we don't wait here for that since we setup async conversion
		last_temp_request_ms = millis();
	}

	long now_ms = millis();

	// read current input status
	unsigned char tmp_status = digitalRead(GPIO_IN_STATUS);
	// invert if needed
	if (IN_STATUS_INVERTED)
		tmp_status = !tmp_status;

	// check if unstable
	if (tmp_status != last_read_status) {
		last_status_unstable_ms = now_ms;
		last_read_status = tmp_status;
	}

	// check if debounce period elapsed, save new status
	if ((now_ms - last_status_unstable_ms) > DEBOUNCE_STATUS_MS) {
		if (current_pc_status != tmp_status) {
			current_pc_status = tmp_status;
			Publish_PcStatus(current_pc_status);
			Serial.print("PC status ");
			Serial.println(current_pc_status);
		}
	}

	// limit minimal MQTT sending interval, publish data
	if ((now_ms - last_published_ms) > PUB_MIN_MS) {
		// send on PC status change
		if (current_pc_status != last_published_status) {
			Publish_PcStatus(current_pc_status);
			last_published_ms = now_ms;
		}	else if (fabs(current_temp - last_published_temp) > PUB_TEMP_THRESHOLD) {
			// or on temperature change
			Publish_Temperature(current_temp);
			last_published_ms = now_ms;
		} else if ((now_ms - last_published_ms) > PUB_PERIODIC_MS) {
		// send periodically
			// cycle through data to publish periodically
			if (data_to_publish >= PUB_DATA_MAX) data_to_publish = PUB_DATA_PC_STATUS;

			if (data_to_publish == PUB_DATA_PC_STATUS)
			    Publish_PcStatus(current_pc_status);
			else if (data_to_publish == PUB_DATA_TEMP)
			    Publish_Temperature(current_temp);

			data_to_publish = (PubData_e)(data_to_publish + 1);
			last_published_ms = now_ms;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// MQTT functions
///////////////////////////////////////////////////////////////////////////////
void Mqtt_Reconnect() {
	char tmp_str[30];
	uint32_t millis_start = millis();

	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		//if (client.connect(MQTT_CLIENT_ID, CLOUDMQTT_USER, CLOUDMQTT_PASS)) {
		if (client.connect(MQTT_CLIENT_ID)) {
			// Once connected, publish an announcement...
			snprintf(tmp_str, sizeof(tmp_str), "%s", MQTT_CLIENT_ID);
			Serial.println(tmp_str);
			Publish_Connection(tmp_str);
			// ... and resubscribe
			client.subscribe(TOPIC_IN_PC_STATE);
			client.subscribe(TOPIC_IN_PC_RESET);
			client.subscribe(TOPIC_IN_PC_HSHUT);
			break;
		} else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}

		// restart ESP if we cannot connect for too long
		if ((millis() - millis_start) > 2 * 60000) {
			Serial.println("Cannot connect to MQTT, restarting...");
			ESP.restart();
		}
	}
}

void Publish_Connection(char * text) {
	// report to terminal for debug
	snprintf(mqtt_msg, sizeof(mqtt_msg), "Publishing %s %s", TOPIC_OUT_CONN, text);
	Serial.println(mqtt_msg);
	// publish to MQTT server
	client.publish(TOPIC_OUT_CONN, text, MQTT_USE_RETAIN);
}

void Publish_PcStatus(unsigned char pc_status) {
	// report to terminal for debug
	snprintf(mqtt_msg, sizeof(mqtt_msg), "%s", String(pc_status).c_str());
	Serial.print("Publishing /status ");
	Serial.println(mqtt_msg);
	// publish to MQTT server
	client.publish(TOPIC_OUT_PC_STATUS, mqtt_msg, MQTT_USE_RETAIN);
	// save last published status
	last_published_status = pc_status;
}

void Publish_Temperature(float temp) {
	int16_t temp_i16;
	char temp_string[10];
	// temperature value is in degrees C as float, we'll report with
	// only one decimal since accuracy of sensor is +-0.5 degC
	// convert to integer to avoid float sprintf issues
	if (temp >= 0.0)
		temp_i16 = (temp + 0.05f); // add 0.05f to round up
	else
		temp_i16 = (temp - 0.05f);
	// write integer temperature to string
	snprintf(temp_string, sizeof(temp_string), "%d", temp_i16);
	// report to terminal for debug
	snprintf(mqtt_msg, sizeof(mqtt_msg), "Publishing /temp %s", temp_string);
	Serial.println(mqtt_msg);
	// publish to MQTT server
	client.publish(TOPIC_OUT_TEMP, temp_string, MQTT_USE_RETAIN);
	// remember last sent
	last_published_temp = temp;
}

void Subscription_Callback(char* topic, unsigned char* payload, unsigned int length) {
	// report to terminal for debug
	Serial.print("MQTT msg arrived: ");
	Serial.print(topic);
	Serial.print(" ");
	for (int i = 0; i < length; i++)
		Serial.print((char)payload[i]);
	Serial.println();

	// check if we received our IN topic for state change
	if (strcmp(TOPIC_IN_PC_STATE, topic) == 0) {
		unsigned char target_state = current_pc_status;
		// now the switch in the app is sending 'true' or 'false'
		// check for valid values
		if ((char)payload[0] == '1') {
				Serial.println("/state 1");
			target_state = 1;
		} else if ((char)payload[0] == '0') {
				Serial.println("/state 0");
			target_state = 0;
		}

		Serial.print("Checking state! target|current=");
		Serial.print(target_state);
		Serial.print("|");
		Serial.println(current_pc_status);
		// toggle output (PC power switch) if we need to change state
		if (target_state != current_pc_status) {
			if (target_state == 1)
				TogglePc(TOGGLE);
			else if (target_state == 0)
				TogglePc(TOGGLE);
		}
	} else if (strcmp(TOPIC_IN_PC_RESET, topic) == 0) {
		if ((char)payload[0] == '1')
				Serial.println("/reset");

		Serial.print("Checking state! current=");
		Serial.println(current_pc_status);
		if (current_pc_status == 1)
			ResetPc();

	} else if (strcmp(TOPIC_IN_PC_HSHUT, topic) == 0) {
		if ((char)payload[0] == '1')
				Serial.println("/hshut");

		Serial.print("Checking state! current=");
		Serial.println(current_pc_status);
		if (current_pc_status == 1)
			TogglePc(TOGGLE_HARD_OFF);
	}
}

void TogglePc(int state) {
	digitalWrite(GPIO_OUT_PWSW, OUT_STATE_ACTIVE);
	delay(state);
	digitalWrite(GPIO_OUT_PWSW, OUT_STATE_INACTIVE);
	Serial.println("Power switched!");
}
void ResetPc(void) {
	digitalWrite(GPIO_OUT_RTSW, OUT_STATE_ACTIVE);
	delay(TOGGLE);
	digitalWrite(GPIO_OUT_RTSW, OUT_STATE_INACTIVE);
	Serial.println("Reset!");
}
