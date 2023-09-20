#!/bin/bash

rm *.txt
rm shared/*.txt

KEM_ALG=$1
docker cp ./socket_client_bob.py client_b:/opt/socket/ # copy socket python file into docker
docker cp ./socket_client_alice.py client_b:/opt/socket/ # copy socket python file into docker

./../kat_kem $KEM_ALG client_b
mv public_key.txt shared/


docker cp shared/public_key.txt client_b:/opt/socket/
docker exec -it client_b python3 /opt/socket/socket_client_bob.py quit # send public key to alice
docker exec -it client_b python3 /opt/socket/socket_client_alice.py cipher_text.txt quit # get cipher text from alice
docker cp client_b:/opt/socket/cipher_text.txt ./shared

#./../kat_kem $KEM_ALG client_b shared/cipher_text.txt secret_key.txt
./../kat_kem $KEM_ALG client_b shared/cipher_text.txt secret_key.txt

#cat decrypted.txt | docker run --rm -it openquantumsafe/curl curl -k https://13.125.30.230:4433/qkey?v=
#xxd decryted.txt | docker run --rm -it openquantumsafe/curl curl -k https://13.125.30.230:4433/qkey?v=
#docker run --rm openquantumsafe/curl curl -k https://13.125.30.230:4433/qkey?v=
output=$(python3 encode.py decrypted.txt)
echo $output
docker run --rm -it openquantumsafe/curl curl -k https://13.125.30.230:4433/qkey?$1=$output > plaintext.txt