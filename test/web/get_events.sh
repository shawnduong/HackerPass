#!/bin/sh
# Usage: ./get_events.sh

curl http://127.0.0.1:8080/api/hp/event \
	-X GET -H "Content-Type: application/json" \
	-d "{\"key\": 0}"
