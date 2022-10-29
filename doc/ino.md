# Functions: ino

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

`uint16_t code`
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

`byte buffer[12]`
`byte len`

12-byte buffer used to read the bytes of a card ID into, and the length of the read card ID. MIFARE Classics can have card ID lengths of 4 or 7 bytes, with some documentation from the [MFRC522 library](https://github.com/miguelbalboa/rfid) suggesting up to 10-byte UIDs. HackerPass currently uses 4-byte UIDs, and `len` is therefore not used.

`len` may be removed in a future optimization.¹

*Timing Variables*

`uint64_t ti`

`ti` (time initial) is used for timing of cache sending, measured in milliseconds. `ti` is taken after setup is complete and refreshed after the send cache is flushed to the API. The minimum time between send flushes is 10 seconds, after which all user card IDs in the cache will send to the API at once.

`ti` may reduce antenna awake time in a future optimization.²

*Card ID Caches*

`uint32_t eventIDs[MAX_EVENTS]`
`uint16_t lenEventIDs`
`uint32_t userIDs[MAX_USERS]`
`uint16_t lenUserIDs`
`uint32_t provisionerIDs[MAX_PROVISIONERS]`
`uint16_t lenProvisionerIDs`
`uint32_t cacheUserIDs[USER_ID_CACHE_SIZE]`
`uint16_t lenCacheUserIDs`

Caches are used for events, users, provisioners, and users who have tapped their cards. This reduces the amount of Wi-Fi transmissions and receptions necessary between the unit and the API. Each of these card 4-byte card IDs. The events, users, and provisioners caches are defined when first setting up and will be updated when an invalid card is scanned. The users cache will additionally be updated while in provision mode after a new card is provisioned, or when it is found that a user already exists but is not in the cache.³

There exists a cache of user IDs that have scanned their card. This will be flushed to the API as a series of attendance registrations between the HackerPass unit's associated event ID and every card scanned in the cache, then cleared.⁴ This flushing and clearing may also be done upon the cache reaching its maximum capacity.

## [`config.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/config.h)
## [`upload.sh`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/upload.sh)
## [`include/algos.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/include/algos.h)
## [`include/api.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/include/api.h)
## [`include/led.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/include/led.h)
## [`include/networking.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/include/networking.h)
## [`include/rc522.h`](https://github.com/shawnduong/HackerPass/blob/doc/src/ino/HackerPass/include/rc522.h)

## Footnotes

1. Removal of unused `len` variable is possible as HackerPass units are guaranteed to always be 4-byte UIDs.
2. `ti` stores the time of either setup completion or the last send cache flush time, whichever is later. It is used so that the send cache can flush to the API a minimum of 10 seconds since the last flush. In the future, this can be used to sleep the Wi-Fi antenna and only wake when flushing, possibly reducing power consumption.
3. It is recognized that updating the user cache upon a card-to-be-provisioned being not found in the cache, but being already found on the API may be a bug. This may not be the case in multi-provisioner setups, but in single-provisioner testing, this may be a race condition or related bug.
   - Replication: enter provision mode and provision a new card. Observe that the card will be provisioned and the cache updated. Tap the card again, and observe that the card was not found in the cache but was found on the API.
4. Flushing the user ID cache to the API to create a series of attendances is currently inefficient as it enacts a series of POST requests. Overhead may be minimized if it enacts one aggregate POST request. The API will also need to be modified to accommodate for this.
