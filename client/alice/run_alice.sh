#!/bin/bash

rm *.txt
rm shared/*.txt

docker run --rm -it openquantumsafe/curl curl -k https://13.125.30.230:4433/qkey?v=$2 > plaintext.txt

KEM_ALG=$1
docker cp ./socket_client_alice.py client_a:/opt/socket/ # copy socket python file into docker
docker cp ./socket_client_bob.py client_a:/opt/socket/ # copy socket python file into docker

docker exec -it client_a python3 /opt/socket/socket_client_alice.py public_key.txt quit # execute socket in docker
docker cp client_a:/opt/socket/public_key.txt ./shared # copy text file from docket to shared folder

start=`date +%s.%N`
./../kat_kem $KEM_ALG client_a plaintext.txt shared/public_key.txt # execute kat_kem for shared_secret & cipher
finish=`date +%s.%N`
diff=$( echo "$finish - $start" | bc -l )
res=$?

mv cipher_text.txt shared/ # move cipher to shared folder
docker cp ./shared/cipher_text.txt client_a:/opt/socket/
docker exec -it client_a python3 /opt/socket/socket_client_bob.py quit
if [ $res != 0 ]
then
    rm plaintext.txt
    exit
fi
echo "$KEM_ALG diff: " $diff >> time