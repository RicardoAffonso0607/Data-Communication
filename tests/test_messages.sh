#!/bin/bash

# Simple test script to verify that sender can send multiple messages
# This script connects to the receiver and sends multiple test messages

echo "Testing multiple message delivery..."

# Send first message
(
    sleep 2
    echo "Teste 1" | nc -w 1 127.0.0.1 4444
) &

# Send second message after a delay
(
    sleep 4
    echo "Teste 2" | nc -w 1 127.0.0.1 4444
) &

# Send third message after another delay
(
    sleep 6
    echo "Teste 3" | nc -w 1 127.0.0.1 4444
) &

wait
echo "Test messages sent successfully!"
