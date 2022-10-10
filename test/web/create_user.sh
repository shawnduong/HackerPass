#!/bin/sh
# Usage: ./create_user.sh <CARDID>

curl http://127.0.0.1:8080/api/hp/user/create \
	-X POST -H "Content-Type: application/json" \
	-d "{\"key\": 0, \"cardID\": $1}"
