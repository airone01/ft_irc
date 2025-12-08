#!/usr/bin/env python3

import socket
import time

HOST = "127.0.0.1"
PORT = 6667
CMD_COUNT = 5000

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    
    # Handshake
    s.sendall(b"CAP LS 302\r\nNICK tester\r\nUSER tester 0 * :Test\r\n")
    
    # Send spam
    print(f"Sending {CMD_COUNT} PINGs...")
    start = time.time()
    for i in range(CMD_COUNT):
        msg = f"PING :token{i}\r\n"
        s.sendall(msg.encode())
        
        # KEY CHANGE: Read periodically to prevent deadlock
        # We use non-blocking check or just read small chunks
        s.setblocking(0)
        try:
            while True:
                data = s.recv(4096)
                if not data: break
        except BlockingIOError:
            pass # No data waiting, keep sending
        s.setblocking(1)

    print("Finished sending. Draining response...")
    
    # Read remaining data
    s.settimeout(2.0)
    total_bytes = 0
    try:
        while True:
            data = s.recv(4096)
            if not data: break
            total_bytes += len(data)
    except socket.timeout:
        print("Timeout reached (expected).")
        
    duration = time.time() - start
    print(f"Test done. Received {total_bytes} bytes in {duration:.2f}s")
    s.close()

except Exception as e:
    print(f"Error: {e}")
