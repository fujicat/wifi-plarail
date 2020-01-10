#include <WiFi.h>

const char ssid[] = "ESP32AP-WiFi";
const char pass[] = "esp32apwifi";
const IPAddress ip(192,168,1,1);
const IPAddress subnet(255,255,255,0);

const int MAX_CLIENT = 5;

const int cin[MAX_CLIENT] = {1,2,3,4,5}; // 車両速度操作用スライドボリューム入力ピン
WiFiClient clients[MAX_CLIENT]; // クライアント（各車両）
int clientCount = 0; // 接続済みクライアント数

WiFiServer server(1234);

void setup() {
	Serial.begin(9600);

	WiFi.softAP(ssid, pass);
	delay(100);
	WiFi.softAPConfig(ip, ip, subnet);

	IPAddress myIP = WiFi.softAPIP();

	pinMode(swin, INPUT);
	for (int i=0; i<5; i++) {
		pinMode(cin[i], INPUT);
	}
	delay(10);

	server.begin();

	Serial.print("SSID: ");
	Serial.println(ssid);
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	Serial.println("Server start!");
}

void loop() {
	WiFiClient client = server.available();

	if (client) {
		String currentLine = "";
		Serial.println("New Client.");

		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				Serial.write(c);
				if (c == '\n') {
					if (currentLine.length() == 0) {
						client.println("HTTP/1.1 200 OK");
						client.println("Content-type:text/html");
						client.println();

						client.print(html);
						client.println();
						break;
					} else {
						currentLine = "";
					}
				} else if (c != '\r') {
					currentLine += c;
				}

				if (currentLine.endsWith("GET /?fo")) {
					stop();
					digitalWrite(0, HIGH);
					digitalWrite(4, HIGH);
				}
				if (currentLine.endsWith("GET /?le")) {
					stop();
					digitalWrite(0, HIGH);
				}
				if (currentLine.endsWith("GET /?ri")) {
					stop();
					digitalWrite(4, HIGH);
				}
				if (currentLine.endsWith("GET /?ba")) {
					stop();
					digitalWrite(2, HIGH);
					digitalWrite(5, HIGH);
				}
				if (currentLine.endsWith("GET /?bl")) {
					stop();
					digitalWrite(2, HIGH);
				}
				if (currentLine.endsWith("GET /?br")) {
					stop();
					digitalWrite(5, HIGH);
				}
				if (currentLine.endsWith("GET /?st")) {
					stop();
				}
			}
		}
		client.stop();
		Serial.println("Client Disconnected.");
	}
}