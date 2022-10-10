#!/bin/sh
# Usage: ./update_event_points.sh <ID> <POINTS>

curl http://127.0.0.1:8080/api/hp/event/update \
	-X POST -H "Content-Type: application/json" \
	-d "{\"key\": 0, \"id\": $1, \"points\": $2}"
