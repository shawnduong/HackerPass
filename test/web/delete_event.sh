#!/bin/sh
# Usage: ./delete_event.sh <ID>

curl http://127.0.0.1:8080/api/event/delete \
	-X POST -H "Content-Type: application/json" \
	-d "{\"id\": $1}"