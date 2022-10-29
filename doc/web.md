# Documentation: web

**web** contains all the web application API, databases, and front-end for the HackerPass system.

Root: [`/src/web/`](https://github.com/shawnduong/HackerPass/blob/doc/src/web/)

Contents:
- [Keys](#keys)
- [Database](#database)
- [HP API](#hp-api)
- [AJAX API](#ajax-api)
- [Footnotes](#footnotes)

## Keys

Related file(s): [`keys.py`](https://github.com/shawnduong/HackerPass/blob/main/src/web/keys.py)

**Definitions**

`FLASK_SECRET_KEY`

Some secret Flask key. It can be anything, and is usually random.

`HP_KEYS`

An array of 64-bit HackerPass unit keys. These should be the same as in each unit's `/src/ino/HackerPass/config.h`. These should be sufficiently unpredictable to protect against bruteforce attacks. Knowing an HP key may authorize an attacker to make unauthorized requests.¹

## Database

Related file(s): [`models.py`](https://github.com/shawnduong/HackerPass/blob/main/src/web/models.py) (generates: `database.sqlite`)

**Tables**

*user* (from `class User(UserMixin, db.Model)`)

`User`:
- A definition for a single user, consisting of a unique card ID and points.
- *Attributes*
  - `id` (Integer)
    - Primary key.
  - `cardID` (Integer)
    - Unique 4-byte card UID associated.
  - `name` (String)
    - Plaintext name as defined by the user.
  - `email` (String)
    - Plaintext email as defined by the user.
  - `points` (Integer)
    - The number of points a user has as the sum of all their attendances.
- *Methods*
  - `__init__(self, cardID=0, name=None, email=None, points=0)`
    - Constructor method for User type objects.
  - `login(cardID: str) -> Union[User, bool]`
    - Check if a cardID string is valid and return the User if so. Else, return False.
  - `update_points(self)`
    - Update the user's points as the sum of their attendances.

*event* (from `class Event(db.Model)`)

`Event`:
- A definition for a single event, consisting of a unique event ID (card ID), points reward, a title, about, room, author(s), and epoch timestamp.
- *Attributes*
  - `id` (Integer)
    - Primary key.
  - `eventID` (Integer)
    - Unique 4-byte card UID associated.
  - `points` (Integer)
    - The number of points rewarded for attending.
  - `title` (String)
    - Title of the event.
  - `about` (String)
    - Description of the event.
  - `room` (String)
    - Room the event takes place in.
  - `author` (String)
    - Author or authors of the event.
  - `start` (Integer)
    - Epoch timestamp of the event start.
  - `end` (Integer)
    - Epoch timestamp of the event end.
- *Methods*
  - `__init__(self, eventID=0, points=0, title="", about="", room="", author="", start=0, end=0)`
    - Constructor method for Event type objects.

*attendance* (from `class Attendance(db.Model)`)

`Attendance`:
- A definition for a single attendance, relating a User to an Event.
- *Attributes*
  - `id` (Integer)
    - Primary key.
  - `user` (Integer)
    - User `id`.
  - `event` (Integer)
    - Event `id`.
- *Methods*
  - `__init__(self, user=0, event=0)`
    - Constructor method for Attendance type objects.

*provisioner* (from `class Provisioner(db.Model)`)

`Provisioner`:
- A definition for a single provisioner, a card that toggles between provision and attendance mode.
- *Attributes*
  - `id` (Integer)
    - Primary key.
  - `cardID` (Integer)
    - Unique 4-byte card UID associated.
- *Methods*
  - `__init__(self, cardID=0)`
    - Constructor method for Provisioner type objects.

## HP API

Related file(s): [`api.py`](https://github.com/shawnduong/HackerPass/blob/main/src/web/api.py)

HackerPass (HP) API calls are enacted by HackerPass units. HackerPass units must specify their `HP_KEY` as the `key` request argument (`?key=<HP_KEY>`) in the API call, or else authentication will fail and the call will be rejected.

GET `/api/hp/user` \
`get_users()`

Return a list of all user card IDs.

Example response:
```js
{
  "CardIDs": [
    3735928559,
    3737844653,
    4276994270
  ],
  "Status": "Success."
}
```

GET `/api/hp/user/<cardID>` \
`get_user(cardID)`

Return everything about a specific user.

Example response:
```js
{
  "3735928559": {
    "cardID": 3735928559,
    "email": null,
    "id": 1,
    "name": null,
    "points": 500
  },
  "Status": "Success."
}
```

POST `/api/hp/user/create` \
`create_user()`

Create a new user with some card ID. Name and email fields are filled out by the user later on after registration.

Fields: `{"cardID": <int cardID>}`

Example response:
```js
{
  "Status": "User created."
}
```

GET `/api/hp/event` \
`get_events()`

Return a JSON list of all events.

Example response:
```js
{
  "Events": [
    {
      "about": "Learn how to make sacrifices to old gods.",
      "author": "Lucy",
      "end": 1668294000,
      "eventID": 322376503,
      "id": 1,
      "points": 500,
      "room": "COB2 110",
      "start": 1668286800,
      "title": "Cult Offerings 101"
    },
    {
      "about": "Learn how to make Linux run on anything.",
      "author": "Anonymous",
      "end": 1668294000,
      "eventID": 305402420,
      "id": 2,
      "points": 250,
      "room": "COB2 120",
      "start": 1668290400,
      "title": "How to Install Linux on a Dead Badger"
    }
  ],
  "Status": "Success."
}
```

GET `/api/hp/event/ids` \
`get_event_ids()`

Return a JSON list of all event IDs.

Example response:
```js
{
  "CardIDs": [
    305402420,
    322376503
  ],
  "Status": "Success."
}
```

POST `/api/hp/event/create` \
`create_event()`

Create a new event with some number of associated points.

Fields:
```js
{
	"eventID": <int cardID>,
	"points": <int points>,
	"title": <String title>,
	"about": <String about>,
	"room": <String room>,
	"author": <String author>,
	"start": <int epochStart>,
	"end": <int epochEnd>
}
```

Example response:
```js
{
  "Status": "Event created."
}
```

POST `/api/hp/event/update` \
`update_event()`

Update an event.

Fields: `{"id": <int ID>, <item>: <value>, ...}`

Note that ID is the database row ID, not the card ID.

Example response:
```js
{
  "Status": "Event updated."
}
```

POST `/api/hp/event/delete` \
`delete_event()`

Delete an event based on its ID.

Fields: `{"id": <int ID>}`

Note that ID is the database row ID, not the card ID.

Example response:
```js
{
  "Status": "Event deleted."
}
```

POST `/api/hp/attendance/create` \
`create_attendance()`

Create an attendance for a user.

Fields: `{"user": <int cardID>, "event": <int cardID>}`

Example response:
```js
{
  "Status": "Attendance created."
}
```

GET `/api/hp/provisioner/ids` \
`get_provisioner_ids()`

Return a JSON list of all provisioner IDs.

Example response:
```js
{
  "CardIDs": [
    286335522,
    2863315899
  ],
  "Status": "Success."
}
```

POST `/api/hp/provisioner/create` \
`create_provisioner()`

Create a new provisioner with some card ID.

Fields: `{"cardID": <int cardID>}`

Example response:
```js
{
  "Status": "Provisioner created."
}
```

## AJAX API

Related file(s): [`ajax.py`](https://github.com/shawnduong/HackerPass/blob/main/src/web/api.py)

AJAX API calls are enacted by users through their web client. They require the user to be logged in.

POST `/ajax/user/update/name` \
`app_user_update_name()`

Update a user's name.

Fields: `{"name": <String name>}`

Example response:
```js
{
  "Status": "Name updated."
}
```

POST `/ajax/user/update/email` \
`app_user_update_email()`

Update a user's email.

Fields: `{"email": <String email>}`

Example response:
```js
{
  "Status": "Email updated."
}
```

GET `/ajax/user/info` \
`app_user_info()`

Return a JSON object of a user's points and the events they've attended as well as upcoming events.

Example response:
```js
{
  "Attendances": [
    {
      "event": 1, 
      "id": 1, 
      "user": 1
    }
  ], 
  "Events": [
    {
      "about": "Learn how to make sacrifices to old gods.", 
      "author": "Lucy", 
      "end": 1668294000, 
      "eventID": 322376503, 
      "id": 1, 
      "points": 500, 
      "room": "COB2 110", 
      "start": 1668286800, 
      "title": "Cult Offerings 101"
    }, 
    {
      "about": "Learn how to make Linux run on anything.", 
      "author": "Anonymous", 
      "end": 1668294000, 
      "eventID": 305402420, 
      "id": 2, 
      "points": 250, 
      "room": "COB2 120", 
      "start": 1668290400, 
      "title": "How to Install Linux on a Dead Badger"
    }
  ], 
  "Points": 500, 
  "Status": "Success."
}
```

## Footnotes

1. Security is not a primary consideration of HackerPass 1.0 as 1.0 is merely a proof-of-concept. However, it will become more important for 2.0.

\* This documentation is rather ugly and poorly formatted. We should find some other method of documenting.
