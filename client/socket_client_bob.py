import socket

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the server's address and port
server_address = ('172.18.0.3', 12345)
client_socket.connect(server_address)
print('Connected to:', server_address)
FLAG = False

while True:
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
    
    filename_request = input('Enter filename request or "quit" to quit: ')
    while True:
        # Request publickey.txt
        client_socket.send(filename_request.encode())

        if filename_request == 'quit':
            FLAG = True
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
    if FLAG:
        break

# Close the connection    
client_socket.close()
