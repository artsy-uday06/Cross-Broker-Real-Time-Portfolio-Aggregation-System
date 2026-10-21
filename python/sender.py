"""sender.py  -  Owner: Parth
Sends text records to the C++ process over a local TCP socket."""

import socket
import time

HOST = "127.0.0.1"
PORT = 5050


def send_records(records, host=HOST, port=PORT, retries=10, delay=1.0):
    """Connect to the C++ receiver (retrying while it starts) and send
    every record as one line. Returns the number of records sent."""
    sock = None
    for attempt in range(1, retries + 1):
        try:
            sock = socket.create_connection((host, port), timeout=5)
            break
        except OSError as err:
            print(f"[sender] connect attempt {attempt}/{retries} failed: {err}")
            time.sleep(delay)
    if sock is None:
        raise ConnectionError("Could not reach the C++ process. Start it first.")

    sent = 0
    with sock:
        for rec in records:
            sock.sendall((rec + "\n").encode("utf-8"))
            sent += 1
    print(f"[sender] sent {sent} records")
    return sent
