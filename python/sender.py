import socket
import time

HOST = "127.0.0.1"
PORT = 5050


# sends all lines to C++ using a TCP socket
def send_records(records):
    sock = None
    for attempt in range(10):
        try:
            sock = socket.create_connection((HOST, PORT))
            break
        except OSError:
            print("[sender] C++ not ready, trying again...")
            time.sleep(1)

    if sock is None:
        print("[sender] could not connect to C++. Start it first.")
        return

    for line in records:
        sock.sendall((line + "\n").encode())
    sock.close()
    print("[sender] sent", len(records), "records")
