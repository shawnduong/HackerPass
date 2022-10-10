#!/bin/sh
# Usage: ./get_users.sh

curl http://127.0.0.1:8080/api/hp/user \
	-X GET -H "Content-Type: application/json" \
	-d "{\"key\": 0}"
