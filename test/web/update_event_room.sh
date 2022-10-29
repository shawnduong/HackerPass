#!/bin/bash
# Usage: ./update_event_room.sh <ID> <ROOM>

curl http://127.0.0.1:8080/api/hp/event/update?key=0 \
	-X POST -H "Content-Type: application/json" \
	-d "{\"id\": $1, \"room\": \"$2\"}"
