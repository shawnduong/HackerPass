#ifndef API_H
#define API_H

#include <ESP8266HTTPClient.h>

/* GET some host+path with a key and write the output to code, response. */
void get(String host, String path, uint32_t key, uint16_t *code, String *response)
{
	WiFiClient client;
	HTTPClient http;
	String url = host + path + "?key=" + String(key);

	http.begin(client, url);
	*code = http.GET();
	*response = http.getString();
	http.end();
}

#endif
