#ifndef NETWORK_H
#define NETWORK_H

#include <ESP8266WiFi.h>

#define POLL_INTERVAL 100  // ms

/* Connect to a Wi-Fi network using its SSID and password. If after timeout ms,
   Wi-Fi connection is unsuccessful, return false. */
bool connect(char *ssid, char *password, uint16_t timeout)
{
	WiFi.begin(ssid, password);

	for (uint8_t i = 0; i < timeout/POLL_INTERVAL; i++)
	{
		if (WiFi.status() == WL_CONNECTED)  return true;
		delay(POLL_INTERVAL);
	}

	return false;
}

/* Return true if connected to a Wi-Fi network. */
bool is_connected()
{
	return (WiFi.status() == WL_CONNECTED);
}

#endif
