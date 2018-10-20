#define GPIO_OUT_PWSW     4  //  // used as output, for toggling transistor that turns on/off the pc
#define GPIO_OUT_RTSW     16  //  // used as output, for toggling transistor that resets the pc
#define GPIO_IN_STATUS  17  //  // used as input for PC on/off status
#define GPIO_TEMP					2		//
#define OUT_STATE_ACTIVE    1   // use define so it's simple to change if signal should be active high or low (NPN or PNP BJT)
#define OUT_STATE_INACTIVE  (!OUT_STATE_ACTIVE)

#define TOGGLE  1000 // how long to keep the pwr pin (GPIO_OUT_SW) active when triggered, we to send signal of just small duration
#define TOGGLE_HARD_OFF  1000 * 5// how long to keep the pwr pin (GPIO_OUT_SW) active when triggered, lets "hold" the power button for 5s

#define IN_STATUS_INVERTED  true // true if reading 1 on input actually means the PC is off, PWR_LED is switching the transistor

#define WIFI_SSID   "Your SSID"
#define WIFI_PASS   "Your WiFi password"

#ifdef USE_SSL

#define SSL true
#if defined (MQTT_OVER_WEBSOCKETS)
#define CLOUDMQTT_PORT	 443
#else
#define CLOUDMQTT_PORT	 8883
#endif

#else

#define SSL false
#if defined (MQTT_OVER_WEBSOCKETS)
#define CLOUDMQTT_PORT	 80
#else
#define CLOUDMQTT_PORT	 1883
#endif

#endif

#define CLOUDMQTT_SERVER "mqtt-xklima22.azurewebsites.net"
#define CLOUDMQTT_USER   ""
#define CLOUDMQTT_PASS   ""
#define ROOT_TOPIC  	 ""
#define CLOUDMQTT_PATH "/mqtt"
#define MQTT_CLIENT_ID   "lolin32"  // note: if you have multiple devices, assign them different ID's
#define MQTT_CLIENT_CATEGORY "mcu"		// "mcu" or "strip" (strip = https://www.itead.cc/sonoff-pow.html and similar)
#define MQTT_CLIENT_PATH "/" MQTT_CLIENT_CATEGORY "/" MQTT_CLIENT_ID

#define MQTT_USE_RETAIN   false         // set to true if you want last msg for all topics retained on server, so you get it automatically on client connect

#define TOPIC_OUT_CONN       ROOT_TOPIC "/conn"
#define TOPIC_OUT_PC_STATUS  ROOT_TOPIC MQTT_CLIENT_PATH "/status"
#define TOPIC_OUT_TEMP       ROOT_TOPIC MQTT_CLIENT_PATH "/temp"
#define TOPIC_IN_PC_STATE    ROOT_TOPIC MQTT_CLIENT_PATH "/state"
#define TOPIC_IN_PC_HSHUT    ROOT_TOPIC MQTT_CLIENT_PATH "/hshut"
#define TOPIC_IN_PC_RESET    ROOT_TOPIC MQTT_CLIENT_PATH "/reset"

#define PUB_PERIODIC_MS     1000 * 60 * 1  // publish data periodically every 1 minute
#define PUB_TEMP_THRESHOLD  2.0f            // besides periodically, temperature will be transmitted if changed more than this
#define PUB_MIN_MS          1000            // don't publish more often that this, even on status/temp change

#define DEBOUNCE_STATUS_MS    1000 * 2   // 2 second debounce for PC on/off status
#define TEMP_REFRESH_MS       1000 * 20   // periodic read of temperature

// enum to cycle through all data that is to be sent periodically
typedef enum
{
	PUB_DATA_PC_STATUS = 0,
	PUB_DATA_TEMP = 1,
	PUB_DATA_MAX = 2
} PubData_e;

void Wifi_Connect(void);
void setup();
void loop();

///////////////////////////////////////////////////////////////////////////////
/// MQTT functions
///////////////////////////////////////////////////////////////////////////////
void Mqtt_Reconnect();
void Publish_Connection(char * text);
void Publish_PcStatus(unsigned char pc_status);
void Publish_Temperature(float temp);
void Subscription_Callback(char* topic, unsigned char* payload, unsigned int length);
void TogglePc(int state);
void ResetPc(void);
