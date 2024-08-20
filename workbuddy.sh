#!/bin/bash
set -a
source .env
set +a
python python/socket_server.py 

while ! nc -z localhost 5000; do   
  sleep 1
done

./WorkBuddy