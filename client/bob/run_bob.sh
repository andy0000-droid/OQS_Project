#!/bin/bash

KEM_ALG=$1
docker cp ./socket_client_bob.py client_b:/opt/socket/ # copy socket python file into docker
docker cp ./socket_client_alice.py client_b:/opt/socket/ # copy socket python file into docker

./../kat_kem $KEM_ALG client_b

docker cp ./public_key.txt client_b:/opt/socket/
docker exec -it client_b python3 /opt/socket/socket_client_bob.py quit # send public key to alice
docker exec -it client_b python3 /opt/socket/socket_client_alice.py cipher_text.txt quit # get cipher text from alice
docker cp client_b:/opt/socket/cipher_text.txt ./shared