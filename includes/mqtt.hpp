# pragma once

# include <PubSubClient.h>
# include <RCSwitch.h>

# include "../includes/topics-mqtt.hpp"
# include "../secret.hpp"

bool POWER = false;

WiFiClient espClient;
PubSubClient client(espClient);
RCSwitch mySwitch = RCSwitch();

void reconnect() {
	while (!client.connected()) {
		if (client.connect("ESP32Client", MQTT_USERNAME, MQTT_PASSWORD)) {
			Serial.println("connected");
			client.subscribe(mqtt_setOn);
			client.subscribe(windobe);
			client.subscribe(is_power);

		} else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			delay(5000);
		}
	}
}
