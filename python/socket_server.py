import socket
import threading
import json
from pyannote.main import get_diarization


def handle_client(client_socket):
    try:
        while True:
            request = client_socket.recv(1024).decode("utf-8")
            if not request:
                break

            parts = request.strip().split()
            if len(parts) != 3 or parts[0] != "GET":
                client_socket.send(b"HTTP/1.1 400 Bad Request\r\n\r\n")
                continue

            route, filepath = parts[1], parts[2]
            route = route.lstrip("/")

            if route == "pyannote":
                response = get_diarization(filepath)
                client_socket.send(json.dumps(response).encode("utf-8"))
            else:
                client_socket.send(b"HTTP/1.1 404 Not Found\r\n\r\n")
    finally:
        client_socket.close()


def start_server(host="0.0.0.0", port=5000):
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((host, port))
    server.listen(5)
    print(f"Server listening on {host}:{port}")

    while True:
        client_socket, addr = server.accept()
        print(f"Accepted connection from {addr}")
        client_handler = threading.Thread(target=handle_client, args=(client_socket,))
        client_handler.start()


if __name__ == "__main__":
    start_server()
