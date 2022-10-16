#!/bin/sh
# Usage: ./update_event_points.sh <ID> <POINTS>

curl http://127.0.0.1:8080/api/hp/event/update?key=0 \
	-X POST -H "Content-Type: application/json" \
	-d "{\"id\": $1, \"points\": $2}"
