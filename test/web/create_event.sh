#!/bin/sh
# Usage: ./create_event.sh <EVENTID> <POINTS> <TITLE> <ABOUT> <ROOM> <AUTHOR> <START> <END>

curl http://127.0.0.1:8080/api/hp/event/create?key=0 \
	-X POST -H "Content-Type: application/json" \
	-d "{\"eventID\": $1, \"points\": $2, \"title\": \"$3\", \"about\": \"$4\", \"room\": \"$5\", \"author\": \"$6\", \"start\": $7, \"end\": $8}"
