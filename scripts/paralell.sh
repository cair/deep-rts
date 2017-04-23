N=$1
echo "Spawning $1 processes"
cd ../build/
for i in {1..40} ;
do
    ./DeepRTS &
done
