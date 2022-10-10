#!/bin/sh
# Usage: ./get_user.sh <CARDID>

curl http://127.0.0.1:8080/api/hp/user/$1 \
	-X GET -H "Content-Type: application/json" \
	-d "{\"key\": 0}"
