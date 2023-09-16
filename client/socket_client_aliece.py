import socket
import subprocess

# Create a socket object
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# Bind the socket to a specific address and port
server_address = ('172.18.0.3', 12345)
server_socket.bind(server_address)

# Listen for incoming connections
server_socket.listen(1)
print('Waiting for a connection...')

# Accept a client connection
client_socket, client_address = server_socket.accept()
print('Connected to:', client_address)
FLAG = False

while True:
    filename_request = input('Enter filename request or "quit" to quit: ')
    while True:
        # Request publickey.txt
        client_socket.send(filename_request.encode())

        if filename_request == 'quit':
            break

        print(f'Request file: {filename_request}')

        # Receive response from the server (file content) 
        received_data = b''
        while True:
            chunk_data=client_socket.recv(1024)        
            received_data += chunk_data
            print(chunk_data)
            if len(chunk_data)<1024:   #This means all data has been received.
                break

        if received_data == b'error':
            print(f'There are no filename {filename_request}\n')
            filename_request = input('Enter filename request or "quit" to quit: ')
            continue

        with open(filename_request,"wb") as f:
            f.write(received_data)     
        print(f'Received File : "{filename_request}"')
        break
    
    while True:
        #Response ciphertext.txt
        filename = client_socket.recv(1024).decode()
        print(f'Requested file: {filename}')

        if filename == 'quit':
            FLAG = True
            break

        try:
            with open(filename, 'rb') as file:
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
# Close the connection
client_socket.close()
server_socket.close()
