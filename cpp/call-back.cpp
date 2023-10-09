#include "../includes/topics-mqtt.hpp"
#include "../includes/mqtt.hpp"

#define PIN 5

void callback_wake(String messageTemp) {
	if (messageTemp == "off") {
		client.publish(mqtt_getOn, "ON");
		digitalWrite(PIN, HIGH);
		Serial.print("Test-1 on");
	}
	else if (messageTemp == "on") {
		client.publish(mqtt_getOn, "OFF");
		digitalWrite(PIN, LOW);
		delay(500);
		digitalWrite(PIN, HIGH);
		Serial.print("Test-1 off");
	}
}


void callback(char* topic, byte* message, unsigned int length) {
	String messageTemp;
  Serial.print("topic is ");
  Serial.println(topic);

	for (int i = 0; i < length; i++) {
	messageTemp += (char)message[i];
	}

	if (!strcmp(topic, mqtt_setOn))
		callback_wake(messageTemp);
}
