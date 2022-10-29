#!/bin/bash
# Usage: ./create_attendance.sh <CARDID> <EVENTID>

curl http://127.0.0.1:8080/api/hp/attendance/create?key=0 \
	-X POST -H "Content-Type: application/json" \
	-d "{\"user\": $1, \"event\": $2}"
