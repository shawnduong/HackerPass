#include "include/networking.h"
#include "config.h"

bool connected;

void setup()
{
	Serial.begin(9600);
	connected = connect(SSID, PASSWORD, 5000);
}

void loop()
{
	if (connected)  Serial.println("Connected.");
	else            Serial.println("Network failed.");

	delay(1000);
}
