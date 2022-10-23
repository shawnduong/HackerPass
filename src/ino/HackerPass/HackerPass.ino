#include "include/api.h"
#include "include/led.h"
#include "include/networking.h"
#include "include/rc522.h"
#include "config.h"

/* HTTP response code and response data buffers. */
uint16_t code;
String response;

uint64_t eventID = 1;  // Debugging.
uint32_t id;
byte buffer[12];
byte len;

void setup()
{
	Serial.begin(9600);
	led_init();
	rc522_init();
}

void loop()
{
	id = 0;

	/* Check for a network connection. Reconnect if disconnected. */
	if (!is_connected())
	{
		Serial.println("Not connected. Connecting now.");
		Serial.println("MAC: " + get_mac());

		if (connect(SSID, PASSWORD, TIMEOUT))  Serial.println("Connection success.");
		else                                   Serial.println("Connection failure.");
	}

	/* Debugging. */
	if (read_uid(10, buffer, &len))
	{
		id = (buffer[0] << 0x18) | (buffer[1] << 0x10) | (buffer[2] << 0x08) | buffer[3];

		Serial.println("Card detected.");
		Serial.println(id);

		post(API, "/api/hp/attendance/create", HP_KEY, &code, &response,
			"{\"user\":"+String(id)+",\"event\":"+String(eventID)+"}");
		Serial.println(code);
		Serial.println(response);
	}

	delay(1000);
}
