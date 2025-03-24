#include <WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Update.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <esp_task_wdt.h>
# include "includes/topics-mqtt.hpp"
# include "includes/secret.hpp"


#define RELAY_PRESS_DURATION 500
#define REBOOT_INTERVAL 86400000UL // 24h
#define WIFI_CHECK_INTERVAL 30000UL // 30s
#define MQTT_CHECK_INTERVAL 30000UL // 30s
#define WDT_TIMEOUT 10

const int ledPin = 5;

WiFiClient espClient;
PubSubClient client(espClient);
// RCSwitch mySwitch = RCSwitch();

WebServer server(80);

unsigned long startTime;
unsigned long lastWifiCheck = 0;
unsigned long lastMqttCheck = 0;

void debug_log(const String& msg) {
	String time_msg = "[" + String(millis()) + "ms] " + msg;
	if (client.connected()) {
		client.publish(DEBUG, time_msg.c_str());
	}
}

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
			debug_log(String("Mise à jour !"));
			esp_task_wdt_delete(NULL);   // Suppression du watchdog pour l'OTA sinon malheur ^^
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
				debug_log(String("Mise à jour terminée !"));
			} else {
				Update.printError(Serial);
			}
		}
	});
}

void setup_wifi() {
	delay(10);
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(WIFI_SSID);
	debug_log(String("WiFi connected to ") + WIFI_SSID);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
	  delay(500);
	  Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	// Opti pour eviter double appel dans log serial + channel debug mqtt
	IPAddress ip = WiFi.localIP();
	Serial.println(ip);
	debug_log(String("WiFi connected to ") + ip.toString());
}

// void	reboot() {
// 	client.publish(DEBUG, "REBOOT esp32");
// 	debug_log(String("Reboot esp32"));
// }

void reconnect() {
	int max_attempts = 5;
	int attempts = 0;

	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("WiFi down, trying reconnect...");
		debug_log(String("WiFi down, trying reconnect..."));
		WiFi.disconnect();
		WiFi.reconnect();
		delay(1000);
	}
	while (!client.connected() && attempts < max_attempts) {
		attempts++;
		Serial.println("Attempting MQTT connection...");
		debug_log(String("Attempting MQTT connection..."));
		if (client.connect("ESP32Client", MQTT_USERNAME, MQTT_PASSWORD)) {
		  Serial.println("MQTT connected");
		  debug_log(String("MQTT connected"));
		  client.subscribe(windobe);
		  client.subscribe(getWindobe);
		  client.subscribe(is_power);
		  client.subscribe(DEBUG);
		} else {
		  Serial.print("Failed, rc=");
		  String msg = String(client.state());
		  Serial.print(msg);
		  debug_log(msg);
		  Serial.println(" try again in 4 seconds");
		  debug_log(String(" try again in 4 seconds"));
		  delay(4000);
		}
	}
}

void callback(char* topic, byte* message, unsigned int length) {
	String messageTemp((char*)message, length);

	if (messageTemp == "on" && !strcmp(topic, windobe)) {
		digitalWrite(ledPin, LOW);
		delay(RELAY_PRESS_DURATION);
		digitalWrite(ledPin, HIGH);
		debug_log(String("Message ON in windows"));
		client.publish(getWindobe, "on");
	}
}

void checkWiFiAndMQTT() {
	unsigned long currentMillis = millis();
	if (ESP.getFreeHeap() < 20000) {  // Seuil à adapter
		Serial.println("Heap too low, restarting...");
		debug_log(String("Heap too low, restarting... ESP/reboot: low heap"));
		ESP.restart();
	}

	if (currentMillis - lastWifiCheck >= WIFI_CHECK_INTERVAL) {
		lastWifiCheck = currentMillis;
		if (WiFi.status() != WL_CONNECTED) {
		Serial.println("WiFi down, restarting...");
		debug_log(String("WiFi down, restarting..."));
		ESP.restart();
		}
	}

	if (currentMillis - lastMqttCheck >= MQTT_CHECK_INTERVAL) {
		lastMqttCheck = currentMillis;
		if (!client.connected()) {
			reconnect();
		}
	}
}

void setup() {
	Serial.begin(115200);
	delay(100);
	debug_log("Last reset reason: " + String(esp_reset_reason()));
	startTime = millis();

	esp_task_wdt_init(WDT_TIMEOUT, true);  // true = reset du système
	esp_task_wdt_add(NULL);

	setup_wifi();
	setupOTA();
	server.begin();

	client.setServer(MQTT_SERVER, MQTT_PORT);
	client.setCallback(callback);

	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, HIGH);
  }


// 86400000 -> 24h

void loop() {
	esp_task_wdt_reset();
	if (millis() - startTime > REBOOT_INTERVAL) {
	  client.publish(DEBUG, "ESP/reboot: periodic safety reboot");
	  Serial.println("24h passed, rebooting");
	  delay(100);
	  ESP.restart();
	}

	checkWiFiAndMQTT();
	client.loop();
	server.handleClient();
  }
