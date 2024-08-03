#include "../includes/topics-mqtt.hpp"
#include "../includes/mqtt.hpp"

#define PIN 5

void callback_wake(String messageTemp, char* topic) {
	if (messageTemp == "off" && (!strcmp(topic, mqtt_setOn) || !strcmp(topic, windobe))) {
		if (!strcmp(topic, windobe)) {
			client.publish(DEBUG, "Set off on Windob and press 2s in button");
			digitalWrite(PIN, LOW);
			delay(3000);
			digitalWrite(PIN, HIGH);
		}
		client.publish(mqtt_getOn, "off");
		client.publish(getWindobe, "off");
	}
	else if (messageTemp == "on" && !strcmp(topic, mqtt_setOn)) {
		digitalWrite(PIN, LOW);
		delay(500);
		digitalWrite(PIN, HIGH);
		client.publish(DEBUG, "Message on in linux");
		client.publish(mqtt_getOn, "on");
		client.publish(getWindobe, "off");

	}
	else if (messageTemp == "on" && !strcmp(topic, windobe)) {
		digitalWrite(PIN, LOW);
		delay(500);
		digitalWrite(PIN, HIGH);
		client.publish(DEBUG, "Message on in windows");
		client.publish(getWindobe, "on");
		client.publish(mqtt_getOn, "off");
	}
}

void callback(char* topic, byte* message, unsigned int length) {
	String messageTemp;

	for (int i = 0; i < length; i++) {
	messageTemp += (char)message[i];
	}
	// Serial.print("topic is ");
	// Serial.println(topic);
	// Serial.print("Message is ");
	// Serial.println(messageTemp);
	callback_wake(messageTemp, topic);
}
