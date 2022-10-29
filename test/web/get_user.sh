#!/bin/bash
# Usage: ./get_user.sh <CARDID>

curl http://127.0.0.1:8080/api/hp/user/$1?key=0
