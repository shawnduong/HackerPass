#!/bin/sh
# Usage: ./create_event.sh <POINTS> <TITLE> <ABOUT> <ROOM> <AUTHOR>

curl http://127.0.0.1:8080/api/event/create \
	-X POST -H "Content-Type: application/json" \
	-d "{\"points\": $1, \"title\": \"$2\", \"about\": \"$3\", \"room\": \"$4\", \"author\": \"$5\"}"
