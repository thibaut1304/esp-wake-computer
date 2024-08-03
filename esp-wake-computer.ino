#include "include.hpp"

const int ledPin = 5;


WebServer server(80);

void setupOTA() {
	server.on("/update", HTTP_GET, []() {
		if (!server.authenticate(WWW_USERNAME, WWW_PASS)) {
			return server.requestAuthentication();
		}
		server.sendHeader("Connection", "close");
		server.send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Mettre à jour'></form>");
	});

	server.on("/update", HTTP_POST, []() {
		server.sendHeader("Connection", "close");
		server.send(200, "text/plain", (Update.hasError()) ? "Mise à jour échouée!" : "Mise à jour réussie! Redémarrage...");
		ESP.restart();
	}, []() {
		HTTPUpload& upload = server.upload();
		if (upload.status == UPLOAD_FILE_START) {
			Serial.printf("Mise à jour: %s\n", upload.filename.c_str());
			if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
				Update.printError(Serial);
			}
		} else if (upload.status == UPLOAD_FILE_WRITE) {
			if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
				Update.printError(Serial);
			}
		} else if (upload.status == UPLOAD_FILE_END) {
			if (Update.end(true)) {
				Serial.printf("Mise à jour terminée: %u\n", upload.totalSize);
			} else {
				Update.printError(Serial);
			}
		}
	});
	}

unsigned long startTime;

void setup() {
	Serial.begin(115200);
	startTime = millis();
	setup_wifi();
	setupOTA();
	server.begin();
	client.setServer(MQTT_SERVER, MQTT_PORT);
	client.setCallback(callback);
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, HIGH);
	// client.publish("Digital_Write", "HIGH");
}

// 86400000 -> 24h

void loop() {
	if (millis() - startTime > 86400000) {
		if (client.connected()) {
			client.publish(DEBUG, "ESP/reboot");
			Serial.println("Time to reboot and is connected");
			delay(100);
		} else {
			Serial.println("Time to reboot and is not connected");
			delay(100);
		}

		ESP.restart();
	}
	if (!client.connected()) {
		reconnect();
	}
	client.loop();
	server.handleClient();
}
