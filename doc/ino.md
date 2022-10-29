# Documentation: ino

**ino** contains all the firmware and definitions for HackerPass units.

Root: [`/src/ino/HackerPass/`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/)

Contents:
- [`HackerPass.ino`](#hackerpassino)
- [`config.h`](#configh)
- [`upload.sh`](#uploadsh)
- [`include/algos.h`](#includealgosh)
- [`include/api.h`](#includeapih)
- [`include/led.h`](#includeledh)
- [`include/networking.h`](#includenetworkingh)
- [`include/rc522.h`](#includerc522h)
- [Footnotes](#footnotes)

## [`HackerPass.ino`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/HackerPass.ino)

This is the primary entry point for **ino**.

**Includes**
- [`include/algos.h`](#includealgosh)
- [`include/api.h`](#includeapih)
- [`include/led.h`](#includeledh)
- [`include/networking.h`](#includenetworkingh)
- [`include/rc522.h`](#includerc522h)
- [`config.h`](#configh)

**Globals**

*HTTP Response Buffers*

`uint16_t code` \
`String response`

Buffers for a request's HTTP status code and response data. These are used when the HackerPass unit GETs or POSTs something to the API in order to determine success and if applicable, retrieve data for further processing.

*Status Variables*

`uint32_t eventID`

Buffer for the current event associated with a HackerPass unit. An event association is required for a HackerPass unit to function. The lack of an event association implies that the HackerPass unit is not tied to an event, and thus it is unknown where points should be routed for a given user's card scan.

`uint32_t id`

Buffer for any read card ID, whether it be an event, user, or provisioner card.

`bool provisioning`

`true` if the HackerPass unit is in provision mode. `false` if the HackerPass unit is in attendance mode.

*Misc. Buffers*

`byte buffer[12]` \
`byte len`

12-byte buffer used to read the bytes of a card ID into, and the length of the read card ID. MIFARE Classics can have card ID lengths of 4 or 7 bytes, with some documentation from the [MFRC522 library](https://github.com/miguelbalboa/rfid) suggesting up to 10-byte UIDs. HackerPass currently uses 4-byte UIDs, and `len` is therefore not used.

`len` may be removed in a future optimization.¹

*Timing Variables*

`uint64_t ti`

`ti` (time initial) is used for timing of cache sending, measured in milliseconds. `ti` is taken after setup is complete and refreshed after the send cache is flushed to the API. The minimum time between send flushes is 10 seconds, after which all user card IDs in the cache will send to the API at once.

`ti` may reduce antenna awake time in a future optimization.²

*Card ID Caches*

`uint32_t eventIDs[MAX_EVENTS]` \
`uint16_t lenEventIDs` \
`uint32_t userIDs[MAX_USERS]` \
`uint16_t lenUserIDs` \
`uint32_t provisionerIDs[MAX_PROVISIONERS]` \
`uint16_t lenProvisionerIDs` \
`uint32_t cacheUserIDs[USER_ID_CACHE_SIZE]` \
`uint16_t lenCacheUserIDs`

Caches are used for events, users, provisioners, and users who have tapped their cards. This reduces the amount of Wi-Fi transmissions and receptions necessary between the unit and the API. Each of these card 4-byte card IDs. The events, users, and provisioners caches are defined when first setting up and will be updated when an invalid card is scanned. The users cache will additionally be updated while in provision mode after a new card is provisioned, or when it is found that a user already exists but is not in the cache.³

There exists a cache of user IDs that have scanned their card. This will be flushed to the API as a series of attendance registrations between the HackerPass unit's associated event ID and every card scanned in the cache, then cleared.⁴ This flushing and clearing may also be done upon the cache reaching its maximum capacity.

**Functions**

*Setup*

`void setup()`

Tasks:
- Initialize RFID and LED.
  - Run LED test.
- Connect to a network.
- Populate the caches.
- Hold until an event association.

*Loop*

`void loop()`

Loop:
- If disconnected from the network, attempt to reconnect.
- If provisioning, attempt to read a card.
  - If the card is a provisioner card, switch back to attendance mode.
  - If the card is a pre-existing user or event card, do not provision it.
  - If the card is new, provision it and update the cache.
    - Provisioning implies that the card is now ready to be handed to a user.
- If taking attendance, attempt to read a card.
  - If it is a valid user card, add it to the cache if it is not already cached.
    - If the cache is full, flush the cache and then add it if it is not already cached.
  - If it is a valid event card, flush the cache and change the event association.
  - If it is a valid provisioner card, switch to provision mode.⁵
  - If it is not in any cache, blink red and update the cache.
- Flush the send cache to the web as a series of attendances between scanned cards and the associated event.⁴
  - There is a minimum wait time of 10 seconds between each non-prompted flush.²

## [`config.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/config.h)

This contains unit-specific configurations.

**Definitions**

`SSID` \
`PASSWORD`

The network SSID (name) and password. If there is not password, set it to be an empty string.

`TIMEOUT`

How long in milliseconds to wait for a network connection before giving up. This is used at the start of the main loop upon detecting a network disconnect. If no connection is found, continue *unless* a connection is explicitly required (as is the case in an urgent cache max flush).

`API`

The API endpoint. Remember to include `http://`. This has not been tested yet with HTTPS or domain names, and has so far been tested with a direct IP address.⁶ Do not include a trailing slash.

`MAX_EVENTS` \
`MAX_USERS`

Adjustable numbers based on event logistics.

`MAX_PROVISIONERS`

The maximum number of provisioner cards across all organizers.

`USER_ID_CACHE_SIZE`

Size of the send cache.

`HP_KEY`

A unique 64-bit value that should also be set in `/src/web/keys.py`. This should remain a secret.⁶

## [`upload.sh`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/upload.sh)

This is simply a convenient upload script that flashes the firmware to a NodeMCU. It is assumed as settings are already defined in the Arduino IDE.⁷ Alternatively, uploading may be done directly through the Arduino IDE.

**Usage**

```sh
$ ./upload.sh <DEVICE>
```

## [`include/algos.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/include/algos.h)

Auxiliary algorithm definitions.

**Functions**

`int32_t bsearch_id(uint32_t id, uint32_t *arr, uint16_t len)`

Binary search some ID array (uint32\_t types) for some ID. Return the index if found, or -1 if not found.

`int32_t lsearch_id(uint32_t id, uint32_t *arr, uint16_t len)`

Linearly search some ID array (uint32\_t types) for some ID. This should be used instead of bsearch in cases where arr is not sorted.

Likewise, return the index if found, or -1 if not found.

## [`include/api.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/include/api.h)

Auxiliary API definitions.

**Includes**
- [`<ArduinoJson.h>` by Benoit Blanchon](https://github.com/bblanchon/ArduinoJson)
- `<ESP8266HTTPClient.h>` (default library from the ESP Arduino core)

**Functions**

`void get(String host, String path, uint64_t key, uint16_t *code, String *response)`

GET some host+path with a key and write the output to code, response.

`void post(String host, String path, uint64_t key, uint16_t *code, String *response, String data)`

POST JSON data to a host+path with a key and write the output to a code, response.

`get_ids(String host, String path, uint64_t key, uint16_t *len, uint32_t *data)`

Get some list of IDs. Write the length to len and the data to data.

## [`include/led.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/include/led.h)

All functions and definitions related to LEDs.

**Definitions**

`LED_ESP` \
`LED_RGB_R` \
`LED_RGB_G` \
`LED_RGB_B`

Output pin numbers for the ESP built-in LED and external RGB LED.

**Functions**

`void led_init()`

Set all LED pins to be outputs.

`void led_esp_on()`

Turn the ESP LED on.

`void led_esp_off()`

Turn the ESP LED off.

`void led_rgb_red()` \
`void led_rgb_green()` \
`void led_rgb_blue()` \
`void led_rgb_purple()` \
`void led_rgb_white()` \
`void led_rgb_off()`

Turn the external RGB LED to some color, or off.

`void led_rgb_blink_to_red(uint32_t interval)` \
`void led_rgb_blink_to_green(uint32_t interval)` \
`void led_rgb_blink_to_blue(uint32_t interval)` \
`void led_rgb_blink_to_purple(uint32_t interval)` \
`void led_rgb_blink_to_white(uint32_t interval)`

Turn the LED off shortly for 100 ms and then turn it on to some color, returning to the caller after interval-100 ms.

`void led_rgb_blink_red(uint32_t interval)` \
`void led_rgb_blink_green(uint32_t interval)` \
`void led_rgb_blink_blue(uint32_t interval)` \
`void led_rgb_blink_purple(uint32_t interval)` \
`void led_rgb_blink_white(uint32_t interval)`

Evenly turn the LED off and on to some color, returning to the caller after interval.

`void led_rgb_test()`

Run a diagnostic test to make sure the LEDs are working.

## [`include/networking.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/include/networking.h)

Networking-related definitions and functions.

**Definitions**

`POLL_INTERVAL`

The interval to pause for before checking if a Wi-Fi connection is up yet.

**Includes**
- `<ESP8266WiFi.h>` (default library from the ESP Arduino core)
- [`include/led.h`](#includeledh)

**Functions**

`bool connect(char *ssid, char *password, uint16_t timeout)`

Connect to a Wi-Fi network using its SSID and password. If after timeout ms, Wi-Fi connection is unsuccessful, return false.

`bool is_connected()`

Return true if connected to a Wi-Fi network.

`String get_mac()`

Get the MAC address of the ESP8266.

## [`include/rc522.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/include/rc522.h)

**Includes**
- [`<MFRC522.h>` by GithubCommunity](https://github.com/miguelbalboa/rfid)

**Functions**

`void rc522_init()`

Begin SPI and PCD.

`bool read_uid(byte attempts, byte *buffer, byte *len)`

Try to read a UID from a card for a specified number of attempts. Write the UID and length to the buffer, and then return true. If after the specified number of attempts, no card was successfully read, return false.

## Footnotes

The bulk of these footnotes are future directions.

1. Removal of unused `len` variable is possible as HackerPass units are guaranteed to always be 4-byte UIDs.
2. `ti` stores the time of either setup completion or the last send cache flush time, whichever is later. It is used so that the send cache can flush to the API a minimum of 10 seconds since the last flush. In the future, this can be used to sleep the Wi-Fi antenna and only wake when flushing, possibly reducing power consumption.
3. It is recognized that updating the user cache upon a card-to-be-provisioned being not found in the cache, but being already found on the API may be a bug. This may not be the case in multi-provisioner setups, but in single-provisioner testing, this may be a race condition or related bug.
   - Replication: enter provision mode and provision a new card. Observe that the card will be provisioned and the cache updated. Tap the card again, and observe that the card was not found in the cache but was found on the API.
4. Flushing the user ID cache to the API to create a series of attendances is currently inefficient as it enacts a series of POST requests. Overhead may be minimized if it enacts one aggregate POST request. The API will also need to be modified to accommodate for this.
5. This should also flush the cache, and that should be implemented in the future.
6. HTTPS and domain names should be tested soon for a more secure HackerPass.
7. The upload script should define all of the parameters so that the Arduino IDE is not needed for uploading.
