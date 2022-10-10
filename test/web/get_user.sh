#!/bin/sh
# Usage: ./get_user.sh <CARDID>

curl http://127.0.0.1:8080/api/user/$1
