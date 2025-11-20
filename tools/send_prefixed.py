#!/usr/bin/env python3
import socket
import sys

if len(sys.argv) < 2:
    print("Usage: send_prefixed.py 'your message here'")
    sys.exit(1)

msg = sys.argv[1]
data = msg.encode('utf-8')
length = len(data)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    s.connect(('127.0.0.1', 4444))
    # send 4-byte big-endian length
    s.sendall(length.to_bytes(4, byteorder='big'))
    s.sendall(data)
    print(f"Sent {length} bytes")
finally:
    s.close()
