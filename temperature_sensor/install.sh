mkdir memory/build
cd memory/build
cmake -S ..
make 
make install
cd ../../

mkdir scheduler/build
cd scheduler/build
cmake -S ..
make 
make install
cd ../../

mkdir build
cd build
cmake -S ../src
make