# pragma once

# include <PubSubClient.h>
# include <RCSwitch.h>

# include "../includes/topics-mqtt.hpp"
# include "../secret.hpp"
# include <WiFi.h>
# include "wifi.hpp"

WiFiClient espClient;
PubSubClient client(espClient);
RCSwitch mySwitch = RCSwitch();

void	reboot() {
	client.publish(DEBUG, "REBOOT esp32");
}

void reconnect() {
	int max_attempts = 5;
	int attempts = 0;

	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("Reconnecting to WiFi...");
		setup_wifi();  // Tente de reconnecter le WiFi
	}
	while (!client.connected()) {
		if (++attempts > max_attempts) {
			Serial.println("Max MQTT reconnect attempts reached. Will retry later.");
			return;  // Exit if max attempts are reached
		}
		if (client.connect("ESP32Client", MQTT_USERNAME, MQTT_PASSWORD)) {
			Serial.println("MQTT connected");
			client.subscribe(mqtt_setOn);
			client.subscribe(mqtt_getOn);
			client.subscribe(windobe);
			client.subscribe(getWindobe);
			client.subscribe(is_power);
			client.subscribe(DEBUG);
		} else {
			Serial.print("MQTT reconnect failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			delay(5000);
		}
	}
}
