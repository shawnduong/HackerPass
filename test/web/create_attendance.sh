#!/bin/sh
# Usage: ./create_attendance.sh <CARDID> <EVENTID>

curl http://127.0.0.1:8080/api/hp/attendance/create \
	-X POST -H "Content-Type: application/json" \
	-d "{\"key\": 0, \"user\": $1, \"event\": $2}"
