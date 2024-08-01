#include "../includes/topics-mqtt.hpp"
#include "../includes/mqtt.hpp"

#define PIN 5

void callback_wake(String messageTemp, char* topic) {
	// std::string test = "false";
	// if (POWER) test = "true";
	// client.publish(is_power, test.c_str());
	// if (!strcmp(topic, is_power)) {
	// 	if (messageTemp ==  "on")
	// 		POWER = true;
	// 	else if (messageTemp == "off")
	// 		POWER = false;
	// }
	// if (POWER && messageTemp == "on") {
	// 	if (!strcmp(topic, windobe))
	// 		client.publish(getWindobe, "off");
	// 	else if (!strcmp(topic, mqtt_setOn))
	// 		client.publish(mqtt_getOn, "off");
	// 	return ;
	// }
	if (messageTemp == "off" && (!strcmp(topic, mqtt_setOn) || !strcmp(topic, windobe))) {
		POWER = false;
		digitalWrite(PIN, HIGH);
		// client.publish("Digital_Write", "HIGH");
		// delay(100);
		// digitalWrite(PIN, HIGH);
		// client.publish("Digital_Write", "HIGH");
		// delay(3000);
		// digitalWrite(PIN, LOW);
		// client.publish("Digital_Write", "LOW");

		client.publish(DEBUG, "Message off in all topic");
		client.publish(mqtt_getOn, "off");
		client.publish(getWindobe, "off");
	}
	else if (messageTemp == "on" && !strcmp(topic, mqtt_setOn)) {
		POWER = true;
		// digitalWrite(PIN, HIGH);
		// client.publish("Digital_Write", "HIGH");
		// delay(100);
		digitalWrite(PIN, LOW);
		// client.publish("Digital_Write", "HIGH");
		delay(500);
		digitalWrite(PIN, HIGH);
		// client.publish("Digital_Write", "LOW");
		client.publish(DEBUG, "Message on in linux");
		client.publish(mqtt_getOn, "on");
		client.publish(getWindobe, "off");
	}
	else if (messageTemp == "on" && !strcmp(topic, windobe)) {
		POWER = true;
		// digitalWrite(PIN, HIGH);
		// client.publish("Digital_Write", "HIGH");
		// delay(100);
		digitalWrite(PIN, LOW);
		// client.publish("Digital_Write", "LOW");
		delay(500);
		digitalWrite(PIN, HIGH);
		// client.publish("Digital_Write", "HIGH");
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
	Serial.print("topic is ");
	Serial.println(topic);
	Serial.print("Message is ");
	Serial.println(messageTemp);
	callback_wake(messageTemp, topic);
}
