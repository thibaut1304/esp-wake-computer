#include "../includes/topics-mqtt.hpp"
#include "../includes/mqtt.hpp"

#define PIN 5

void callback_wake(String messageTemp, char* topic) {
	if (messageTemp == "off" && !strcmp(topic, mqtt_setOn) ) {
		// client.publish(mqtt_getOn, "ON");
		digitalWrite(PIN, HIGH);
		// Serial.print("Test-1 on");
	}
	else if (messageTemp == "on" && !strcmp(topic, mqtt_setOn)) {
		digitalWrite(PIN, LOW);
		delay(500);
		client.publish(mqtt_getOn, "off");
		digitalWrite(PIN, HIGH);
		// Serial.print("Test-1 off");
	}
	else if (messageTemp == "on" && !strcmp(topic, windobe)) {
		digitalWrite(PIN, LOW);
		delay(500);
		client.publish(getWindobe, "off");
		digitalWrite(PIN, HIGH);
	}
}


void callback(char* topic, byte* message, unsigned int length) {
	String messageTemp;
  Serial.print("topic is ");
  Serial.println(topic);

	for (int i = 0; i < length; i++) {
	messageTemp += (char)message[i];
	}

	callback_wake(messageTemp, topic);
}
