import socket

HOST = '127.0.0.1'
PORT = 7777

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
    client.connect((HOST, PORT))

    while True:
        
        message = input()
        if( message == "exit"):
            break
        client.sendall(message.encode())
        response = client.recv(1024)
        print(f"[CLIENT] Received: {response.decode()}")
