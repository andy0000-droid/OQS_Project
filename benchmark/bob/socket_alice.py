# bob
import socket
import subprocess
import shlex
import sys

argv = sys.argv[1:]

# Create a socket object
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# Bind the socket to a specific address and port
server_address = ('172.19.0.3', 12345)
server_socket.bind(server_address)

# Listen for incoming connections
server_socket.listen(1)
print('Waiting for a connection...')

# Accept a client connection
client_socket, client_address = server_socket.accept()
print('Connected to:', client_address)

FLAG = False
prefix = '/opt/'
i = 0

while True:
    #filename_request = input('Enter filename request or "quit" to quit: ')
    filename_request = argv[i]
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
            #print(chunk_data)
            if len(chunk_data)<1024:   #This means all data has been received.
                break

        if received_data == b'error':
            print(f'There are no filename {filename_request}\n')
            filename_request = input('Enter filename request or "quit" to quit: ')
            continue

        with open(prefix + filename_request,"wb") as f:
            f.write(received_data)     
        print(f'Received File : "{filename_request}"')
        #cmd = ['./kat_kem', 'KEM_ALG', 'client_a', 'plaintext.txt', 'publickey.txt']
        #cmd[1] = 'kyber1024'
        #subprocess.Popen(args = cmd, text=True, shell = True)
        break

    if FLAG:
        break
    else:
        i += 1
    if i > (len(argv) - 1):
        break
# Close the connection

print("Connection ended")
client_socket.close()
server_socket.close()