#include "include/networking.h"
#include "config.h"

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	/* Check for a network connection. Reconnect if disconnected. */
	if (!is_connected())
	{
		Serial.println("Not connected. Connecting now.");
		if (connect(SSID, PASSWORD, TIMEOUT))  Serial.println("Connection success.");
		else                                   Serial.println("Connection failure.");
	}

	delay(1000);
}
