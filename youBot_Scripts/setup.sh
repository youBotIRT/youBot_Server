#!/bin/sh


echo 'Building all Appilcations' 

cd ..

cd youBot_Client
rm build/ -r
rm lib/ -r
mkdir build
cd build
cmake ..
make
cd ../..

cd youBot_Server
rm build/ -r
rm bin/ -r
mkdir build
cd build
cmake ..
make
cd ../..

cd youBot_JoyPad
rm build/ -r
rm bin/ -r
mkdir build
cd build
cmake ..
make
cd ../..

cd youBot_Info
rm build/ -r
rm bin/ -r
mkdir build
cd build
cmake ..
make
cd ../..

cd youBot_Monitor
rm build/ -r
rm bin/ -r
mkdir build
cd build
cmake ..
make
cd ../..

cd youBot_UDP
rm build/ -r
rm bin/ -r
mkdir build
cd build
cmake ..
make
cd ../..

cd youBot_PID_Tuner
rm build/ -r
rm bin/ -r
mkdir build
cd build
cmake ..
make
cd ../..

echo 'Making Ethercat and the LCD Display accesable without Sudo'
sudo setcap cap_net_raw+ep /youBot_Server/bin/youBot_Server
sudo chmod 666 /dev/ttyACM0

