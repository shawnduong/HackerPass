#include "include/algos.h"
#include "include/api.h"
#include "include/led.h"
#include "include/networking.h"
#include "include/rc522.h"
#include "config.h"

/* HTTP response code and response data buffers. */
uint16_t code;
String response;

/* Status variables. */
uint32_t eventID;
uint32_t id;
bool provisioning;

/* Buffers. */
byte buffer[12];
byte len;

/* Timing variables. */
uint64_t ti;

/* Cached eventIDs and userIDs retrieved from the web. These are expected to be
   sorted web-side to offload computational cost as these will be b-searched. */
uint32_t eventIDs[MAX_EVENTS];
uint16_t lenEventIDs;
uint32_t userIDs[MAX_USERS];
uint16_t lenUserIDs;

/* Cached provisionerIDs retrieved from the web, likewise expected to be sorted. */
uint32_t provisionerIDs[MAX_PROVISIONERS];
uint16_t lenProvisionerIDs;

/* Cache of cards to send to the API all at once. */
uint32_t cacheUserIDs[USER_ID_CACHE_SIZE];
uint16_t lenCacheUserIDs;

void setup()
{
	Serial.begin(9600);
	rc522_init();
	led_esp_off();
	led_init();
	led_rgb_test();
	led_rgb_blink_to_red(1000);

	/* Initialize variables. */
	eventID = 0;  // Edge case when actual card ID = 0 (p = 2e-10). p is
	id = 0;       // sufficiently small to justify this micro-optimization.
	provisioning = false;

	/* Initialize cache. */
	lenCacheUserIDs = 0;

	/* Force a network connection to start. */
	Serial.println("Connecting to a network.");
	while (!connect(SSID, PASSWORD, TIMEOUT));
	led_esp_on();

	Serial.println("Getting event IDs from API.");
	get_ids(API, "/api/hp/event/ids", HP_KEY, &lenEventIDs, eventIDs);

	Serial.println("Getting user IDs from API.");
	get_ids(API, "/api/hp/user", HP_KEY, &lenUserIDs, userIDs);

	Serial.println("Getting provisioner IDs from API.");
	get_ids(API, "/api/hp/provisioner/ids", HP_KEY, &lenProvisionerIDs, provisionerIDs);

	/* Wait for an organizer to tap an event association card. This phase lasts
	   for a maximum time of 25.5 s, but will loop until successful. */
	Serial.println("Waiting for event association.");
	while (true)
	{
		if (!read_uid(255, buffer, &len))  continue;
		id = (buffer[0] << 0x18) | (buffer[1] << 0x10) | (buffer[2] << 0x08) | buffer[3];

		if (bsearch_id(id, eventIDs, lenEventIDs) < 0)
		{
			led_rgb_blink_red(200);
		}
		else
		{
			eventID = id;
			led_rgb_blink_to_white(1000);
			break;
		}
	}

	delay(1000);
	Serial.println("Setup complete.");

	ti = millis();
}

void loop()
{
	/* Check for a network connection. Attempt to reconnect if disconnected. */
	if (!is_connected())
	{
		led_esp_off();
		Serial.println("Not connected. MAC: " + get_mac());

		if (connect(SSID, PASSWORD, TIMEOUT))  led_esp_on();
		else                                   led_esp_off();
	}
	else  led_esp_on();

	/* Try to provision a card. This will immediately call to the web. */
	if (provisioning && read_uid(100, buffer, &len))
	{
		id = (buffer[0] << 0x18) | (buffer[1] << 0x10) | (buffer[2] << 0x08) | buffer[3];

		Serial.println("Card detected.");
		Serial.println(id);

		/* Check if it's a valid provision card. Toggle modes if so. */
		if (bsearch_id(id, provisionerIDs, lenProvisionerIDs) >= 0)
		{
			Serial.println("Switching to attendance mode.");
			provisioning = false;
			led_rgb_blink_to_white(1000);
			delay(1000);
		}
		/* Check if it's already a user card. */
		else if (bsearch_id(id, userIDs, lenUserIDs) >= 0)
		{
			Serial.println("Refused to provision card: pre-existing user.");
			led_rgb_blink_green(500);
			led_rgb_blink_to_purple(500);
		}
		/* Check if it's already an event card. */
		else if (bsearch_id(id, eventIDs, lenEventIDs) >= 0)
		{
			Serial.println("Refused to provision card: pre-existing event.");
			led_rgb_blink_blue(500);
			led_rgb_blink_to_purple(500);
		}
		/* Provision the card. */
		else
		{
			post(API, "/api/hp/user/create", HP_KEY, &code, &response, "{\"cardID\":"+String(id)+"}");

			if (code == 200)
			{
				Serial.println("Successfully provisioned card.");
				led_rgb_blink_green(500);
				led_rgb_blink_to_purple(500);

				Serial.println("Updating cache.");
				get_ids(API, "/api/hp/user", HP_KEY, &lenUserIDs, userIDs);
			}
			else
			{
				Serial.println("Could not successfully provision card. HTTP code and response:");
				Serial.println(code);
				Serial.println(response);
				led_rgb_blink_red(500);
				led_rgb_blink_to_purple(500);

				DynamicJsonDocument json(1024);
				deserializeJson(json, response);

				if (json["Status"] == "User already exists.")
				{
					Serial.println("Updating cache.");
					get_ids(API, "/api/hp/user", HP_KEY, &lenUserIDs, userIDs);
				}
			}
		}
	}
	/* Try to read a card. This phase lasts for a maximum time of 10 s. */
	else if (!provisioning && read_uid(100, buffer, &len))
	{
		id = (buffer[0] << 0x18) | (buffer[1] << 0x10) | (buffer[2] << 0x08) | buffer[3];

		Serial.println("Card detected.");
		Serial.println(id);

		/* Check if it's a valid user card. */
		if (bsearch_id(id, userIDs, lenUserIDs) >= 0)
		{
			led_rgb_blink_green(500);
			led_rgb_blink_to_white(500);

			if (lenCacheUserIDs >= USER_ID_CACHE_SIZE)
			{
				Serial.println("Cache is full. Flushing to web.");

				/* It is REQUIRED that there be a network connection! */
				while (!is_connected())
				{
					led_esp_off();
					Serial.println("Not connected. MAC: " + get_mac());

					if (connect(SSID, PASSWORD, TIMEOUT))  led_esp_on();
					else                                   led_esp_off();
				}

				led_esp_on();

				for (uint16_t i = 0; i < lenCacheUserIDs; i++)
				{
					while (true)
					{
						post(API, "/api/hp/attendance/create", HP_KEY, &code, &response,
							"{\"user\":"+String(cacheUserIDs[i])+", \"event\":"+String(eventID)+"}");
						if (code == 400)
						{
							break;
						}
						else if (code != 200)
						{
							delay(1000);
							continue;
						}
						else
						{
							break;
						}
					}
				}

				lenCacheUserIDs = 0;
			}

			/* Add to the send cache if it has not already been cached. */
			if (lsearch_id(id, cacheUserIDs, lenCacheUserIDs) < 0)
			{
				Serial.println("Adding to send cache.");
				cacheUserIDs[lenCacheUserIDs++] = id;
			}
			else
			{
				Serial.println("Card is already a part of the send cache.");
			}
		}
		else if (bsearch_id(id, eventIDs, lenEventIDs) >= 0)
		{
			led_rgb_blink_white(500);

			/* New event implies the need to flush the send cache. */
			if (id != eventID)
			{
				Serial.println("New event association.");
				eventID = id;

				/* It is REQUIRED that there be a network connection! */
				while (!is_connected())
				{
					led_esp_off();
					Serial.println("Not connected. MAC: " + get_mac());

					if (connect(SSID, PASSWORD, TIMEOUT))  led_esp_on();
					else                                   led_esp_off();
				}

				led_esp_on();

				for (uint16_t i = 0; i < lenCacheUserIDs; i++)
				{
					while (true)
					{
						post(API, "/api/hp/attendance/create", HP_KEY, &code, &response,
							"{\"user\":"+String(cacheUserIDs[i])+", \"event\":"+String(eventID)+"}");
						if (code == 400)
						{
							break;
						}
						else if (code != 200)
						{
							delay(1000);
							continue;
						}
						else
						{
							break;
						}
					}
				}

				lenCacheUserIDs = 0;
			}
			else
			{
				Serial.println("Unit is already associated.");
			}
		}
		/* Check if it's a valid provision card. Toggle modes if so. */
		else if (bsearch_id(id, provisionerIDs, lenProvisionerIDs) >= 0)
		{
			Serial.println("Switching to provision mode.");
			provisioning = true;
			led_rgb_blink_to_purple(1000);
			delay(1000);
		}
		/* Invalid card. */
		else
		{
			Serial.println("Invalid card. Updating caches.");
			led_rgb_blink_red(500);
			led_rgb_blink_to_white(500);

			if (is_connected())
			{
				Serial.println("Getting event IDs from API.");
				get_ids(API, "/api/hp/event/ids", HP_KEY, &lenEventIDs, eventIDs);

				Serial.println("Getting user IDs from API.");
				get_ids(API, "/api/hp/user", HP_KEY, &lenUserIDs, userIDs);

				Serial.println("Getting provisioner IDs from API.");
				get_ids(API, "/api/hp/provisioner/ids", HP_KEY, &lenProvisionerIDs, provisionerIDs);
			}
		}
	}

	/* Flush to web and clear cache. This happens approximately every 10 seconds
	   and will only happen if there are items in the cache. */
	if (lenCacheUserIDs > 0 && millis()-ti > 10000 && is_connected())
	{
		Serial.println("Flushing send cache to the web.");

		for (uint16_t i = 0; i < lenCacheUserIDs; i++)
		{
			while (true)
			{
				post(API, "/api/hp/attendance/create", HP_KEY, &code, &response,
					"{\"user\":"+String(cacheUserIDs[i])+", \"event\":"+String(eventID)+"}");
				if (code == 400)
				{
					break;
				}
				else if (code != 200)
				{
					delay(1000);
					continue;
				}
				else
				{
					break;
				}
			}
		}

		lenCacheUserIDs = 0;
		ti = millis();
	}
}
