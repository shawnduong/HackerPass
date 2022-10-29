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

## AJAX API

Related file(s): [`ajax.py`](https://github.com/shawnduong/HackerPass/blob/main/src/web/api.py)

## Footnotes

1. Security is not a primary consideration of HackerPass 1.0 as 1.0 is merely a proof-of-concept. However, it will become more important for 2.0.

\* This documentation is rather ugly and poorly formatted. We should find some other method of documenting.
