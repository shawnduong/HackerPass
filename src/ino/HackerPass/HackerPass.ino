#include "include/api.h"
#include "include/networking.h"
#include "config.h"

/* HTTP response code and response data buffers. */
uint16_t code;
String response;

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

	/* Debugging. */
	get(API, "/api/hp/user", HP_KEY, &code, &response);
	Serial.println(code);
	Serial.println(response);

	delay(1000);
}
