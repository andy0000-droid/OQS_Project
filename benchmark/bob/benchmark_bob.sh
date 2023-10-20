#!/bin/bash

# remove all text file exist
rm *.txt
rm shared/*.txt
rm *.pem

# set KEM algorithm and copy socket file to docker
KEM_ALG="Kyber1024"
docker cp ./socket_bob.py bob_test:/opt/ # copy socket python file into docker
docker cp ./socket_alice.py bob_test:/opt/ # copy socket python file into docker

start=`date +%s.%N`
# code for ECDH key gen
openssl genpkey -out bob.pem -algorithm EC -pkeyopt ec_paramgen_curve:sect571r1 -pkeyopt ec_param_enc:named_curve
openssl pkey -pubout -in bob.pem -out bob.pub
finish=`date +%s.%N`
diff=$( echo "$finish - $start" | bc -l )
###bob has bob_pri & bob_pub

# move public key to shared folder 
mv bob.pub shared/

docker cp ./shared/bob.pub bob_test:/opt/


docker exec -it bob_test python3 /opt/socket_bob.py quit # send public key to alice
docker exec -it bob_test python3 /opt/socket_alice.py alice.pub quit
docker cp bob_test:/opt/alice.pub ./shared
#start=`date +%s.%N`
# code for ECDH key decryption
openssl pkeyutl -derive -inkey bob.pem -peerkey ./shared/alice.pub -out bob_shared_secret.bin

docker exec -it bob_test python3 /opt/socket_alice.py cipher_text.txt quit # get cipher text from alice
docker cp bob_test:/opt/cipher_text.txt ./shared
start=`date +%s.%N`
openssl enc -aes256 -kfile bob_shared_secret.bin -d -in ./shared/cipher_text.txt -out plain_again.txt
finish=`date +%s.%N`
diff1=$( echo "$finish - $start" | bc -l )
if [ $? != 0 ]
then
    exit
fi

echo "$KEM_ALG keygen: " $diff >> time_bob
echo "$KEM_ALG decrypt: " $diff1 >> time_bob