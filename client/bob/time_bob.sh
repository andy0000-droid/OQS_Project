echo "BIKE L5"
for i in 1 2 3 4 5
do  
    sh run_bob.sh BIKE-L5
done

echo "Classic-McEliece-6688128"
for i in 1 2 3 4 5
do 
    sh run_bob.sh Classic-McEliece-6688128
done

echo "Classic-McEliece-6688128f"
for i in 1 2 3 4 5
do 
    sh run_bob.sh Classic-McEliece-6688128f
done

echo "Classic-McEliece-6960119"
for i in 1 2 3 4 5
do 
    sh run_bob.sh Classic-McEliece-6960119
done

echo "Classic-McEliece-6960119f"
for i in 1 2 3 4 5
do 
    sh run_bob.sh Classic-McEliece-6960119f
done

echo "Classic-McEliece-8192128"
for i in 1 2 3 4 5
do 
    sh run_bob.sh Classic-McEliece-8192128
done

echo "Classic-McEliece-8192128f"
for i in 1 2 3 4 5
do 
    sh run_bob.sh Classic-McEliece-8192128f
done

echo "HQC-256"
for i in 1 2 3 4 5
do 
    sh run_bob.sh HQC-256
done

echo "Kyber1024"
for i in 1 2 3 4 5
do 
    sh run_bob.sh Kyber1024
done

echo "FrodoKEM-1344-AES"
for i in 1 2 3 4 5
do 
    sh run_bob.sh FrodoKEM-1344-AES
done

echo "FrodoKEM-1344-SHAKE"
for i in 1 2 3 4 5
do 
    sh run_bob.sh FrodoKEM-1344-SHAKE
done