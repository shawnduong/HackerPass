#!/bin/sh
# Usage: ./create_user.sh <CARDID> <NAME> <EMAIL>

curl http://127.0.0.1:8080/api/user/create \
	-X POST -H "Content-Type: application/json" \
	-d "{\"cardID\": $1, \"name\": \"$2\", \"email\": \"$3\"}"
