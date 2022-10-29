#!/bin/bash
# Usage: ./create_provisioner.sh <CARDID>

curl http://127.0.0.1:8080/api/hp/provisioner/create?key=0 \
	-X POST -H "Content-Type: application/json" \
	-d "{\"cardID\": $1}"
