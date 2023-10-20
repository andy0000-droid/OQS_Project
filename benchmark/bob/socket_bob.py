# bob
import socket
import subprocess
import shlex
import sys
import os

argv = sys.argv[1:]

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the server's address and port
server_address = ('172.19.0.2', 12345)
while True:
    try:
        client_socket.connect(server_address)
        print('Connected to:', server_address)
        break
    except Exception as e:
        os.system('clear')
        print("Waiting server")
        continue

FLAG = False
prefix = '/opt/'
i = 0

while True:
    while True:
        #Response publickey.txt
        filename = client_socket.recv(1024).decode()
        print(f'Requested file: {filename}')

        if filename == 'quit':
            FLAG = True
            break

        try:
            with open(prefix + filename, 'rb') as file:
                content = file.read()
                # Send response back to the client (file content)
                client_socket.send(content)
                print(f'Sent file: {filename}')
                break

        except FileNotFoundError:
            client_socket.send('error'.encode())
            print(f'File not found: {filename}')
            continue
    
    if FLAG:
        break
    else:
        i += 0
    if i > (len(argv) - 1):
        break


print("Connection ended")
# Close the connection    
client_socket.close()