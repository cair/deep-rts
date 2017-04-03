N=$1
echo "Spawning $1 processes"
cd ../c++/build/
for i in {1..40} ;
do
    ( ./DeepRTS & );
done
