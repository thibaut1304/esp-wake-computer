# pragma once

# include <PubSubClient.h>
# include <RCSwitch.h>

# include "../includes/topics-mqtt.hpp"
# include "../secret.hpp"
# include <WiFi.h>
# include "wifi.hpp"

#include <WiFiUdp.h>
bool POWER = false;

WiFiClient espClient;
PubSubClient client(espClient);
RCSwitch mySwitch = RCSwitch();

void reconnect() {
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("Reconnecting to WiFi...");
		setup_wifi();  // Tente de reconnecter le WiFi
	}
	while (!client.connected()) {
		if (client.connect("ESP32Client", MQTT_USERNAME, MQTT_PASSWORD)) {
			Serial.println("connected");
			client.subscribe(mqtt_setOn);
			client.subscribe(windobe);
			client.subscribe(is_power);
			client.subscribe(DEBUG);

		} else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			delay(5000);
		}
	}
}
