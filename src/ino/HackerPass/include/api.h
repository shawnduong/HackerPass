#ifndef API_H
#define API_H

#include <ESP8266HTTPClient.h>

/* GET some host+path with a key and write the output to code, response. */
void get(String host, String path, uint64_t key, uint16_t *code, String *response)
{
	WiFiClient client;
	HTTPClient http;
	String url = host + path + "?key=" + String(key);

	http.begin(client, url);
	*code = http.GET();
	*response = http.getString();
	http.end();
}

/* POST JSON data to a host+path with a key and write the output to a code, response. */
void post(String host, String path, uint64_t key, uint16_t *code, String *response, String data)
{
	WiFiClient client;
	HTTPClient http;
	String url = host + path + "?key=" + String(key);

	http.begin(client, url);
	http.addHeader("Content-Type", "application/json");
	*code = http.POST(data);
	*response = http.getString();
	http.end();
}

#endif
