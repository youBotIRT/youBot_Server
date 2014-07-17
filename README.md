youBot_Server
=============

These libraries are required:

 * [youbot_driver](https://github.com/youbot/youbot_driver)
 * [Boost C++ Libraries](http://www.boost.org/)
 * [ZMQ](http://zeromq.org/)
 * [MSGPACK](http://msgpack.org/)

Installation
------------

Install [youbot_driver](https://github.com/youbot/youbot_driver)

Get Source:

    git clone https://github.com/youBotIRT/youBot_Server.git

Install libaries

    sudo apt-get install libzmq-dev
    sudo apt-get install libmsgpack-dev
    sudo apt-get install libboost-all-dev
    
Setting Up Environment Variables
    
    

Compile youBotIRT

     cd <your_folder>/youBot_Scripts
     ./setup.sh
    
Usage
-----------

The included start script will start:
 * Server
 * JoyPad Node
 * 4 UDP Nodes (Ports 6500 - 6503)
 * Info Node (Dispay)

Start:

     cd <your_folder>/youBot_Scripts
     ./start.sh

Stop:

     cd <your_folder>/youBot_Scripts
     ./stop.sh

