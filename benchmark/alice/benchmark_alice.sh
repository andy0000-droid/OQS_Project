#!/bin/bash

# remove all text file exist
rm *.txt
rm shared/*.txt
rm *.pem


# copy socket file to docker
docker cp ./socket_alice.py alice_test:/opt/ # copy socket python file into docker
docker cp ./socket_bob.py alice_test:/opt/ # copy socket python file into docker

# set KEM algorithm and get plaintext
KEM_ALG="Kyber1024"
docker run --rm -it openquantumsafe/curl curl -k https://13.125.30.230:4433/$KEM_ALG?v > plaintext.txt

#start=`date +%s.%N`
# code for ECDH key gen
start=`date +%s.%N`
openssl genpkey -out alice.pem -algorithm EC -pkeyopt ec_paramgen_curve:sect571r1 -pkeyopt ec_param_enc:named_curve
openssl pkey -pubout -in alice.pem -out alice.pub
finish=`date +%s.%N`
diff=$( echo "$finish - $start" | bc -l )
###alice has alice_pri & alice_pub

# move public key to shared folder
mv alice.pub shared/

docker cp ./shared/alice.pub alice_test:/opt/

#start=`date +%s.%N`
docker exec -it alice_test python3 /opt/socket_alice.py bob.pub quit # execute socket in docker
docker exec -it alice_test python3 /opt/socket_bob.py quit
docker cp alice_test:/opt/bob.pub ./shared/ # copy text file from docket to shared folder

# code for ECDH encryption
openssl pkeyutl -derive -inkey alice.pem -peerkey ./shared/bob.pub -out alice_shared_secret.bin
start=`date +%s.%N`
openssl enc -aes256 -kfile alice_shared_secret.bin -e -in plaintext.txt -out cipher_text.txt

finish=`date +%s.%N`
diff1=$( echo "$finish - $start" | bc -l )
res=$?

mv cipher_text.txt ./shared/ # move cipher to shared folder
docker cp ./shared/cipher_text.txt alice_test:/opt/
docker exec -it alice_test python3 /opt/socket_bob.py quit

if [ $res != 0 ]
then
    rm plaintext.txt
    exit
fi
echo "$KEM_ALG keygen: " $diff >> time_alice
echo "$KEM_ALG encrypt: " $diff1 >> time_alice
