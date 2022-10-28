#ifndef API_H
#define API_H

#include <ArduinoJson.h>  // ArduinoJson library by Benoit Blanchon.
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

/* Get some list of IDs. Write the length to len and the data to data. */
void get_ids(String host, String path, uint64_t key, uint16_t *len, uint32_t *data)
{
	/* 1 KB allocated for JSON data buffer. */
	DynamicJsonDocument json(1024);

	uint16_t code;
	String response;

	while (true)
	{
		delay(500);
		get(host, path, key, &code, &response);

		if (code != 200)
			continue;

		if (deserializeJson(json, response))
			continue;

		if (json["Status"] != "Success.")
			continue;

		*len = json["CardIDs"].size();

		for (uint16_t i = 0; i < *len; i++)
			data[i] = json["CardIDs"][i].as<uint32_t>();

		return;
	}
}

#endif
