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

Related file(s): [`models.py`](https://github.com/shawnduong/HackerPass/blob/main/src/web/models.py)

## HP API

Related file(s): [`api.py`](https://github.com/shawnduong/HackerPass/blob/main/src/web/api.py)

## AJAX API

Related file(s): [`ajax.py`](https://github.com/shawnduong/HackerPass/blob/main/src/web/api.py)

## Footnotes

1. Security is not a primary consideration of HackerPass 1.0 as 1.0 is merely a proof-of-concept. However, it will become more important for 2.0.
