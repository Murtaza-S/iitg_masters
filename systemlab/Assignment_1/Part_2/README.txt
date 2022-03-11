PART 2 :

Prerequist:
Please install g++11 before compiling as **c++ 20 is required**

In linux-------------------------------------------------------------------------------
if g++ is not installed execute these commands:
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt install -y gcc-11
sudo apt install -y g++-11
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 --slave /usr/bin/g++ g++ /usr/bin/g++-11 --slave /usr/bin/gcov gcov /usr/bin/gcov-11
--------------------------------------------------------------------------------------

For Windows visual studio (2021 /2022)-------------------------------------------------------------
In project properties, Under "Configuration Properties > General > C++ Language Standard" set to "ISO C++20 Standard (/std:c++20)"
--------------------------------------------------------------------------------------
Compilation and Running:
The command to compile the run the file use following command:

make

Explaination :

->The program take input no. of guests.

->There are 5 Semaphore that helps in thread synchronization.

->With user input of no. of thread, a random 3 digit unique code is generated for each thread.

->Each thread, randomly pick  the read or write operation in a loop by enqueuing a operation on a priority queue and then sleep

-> Access to priority queue is locked with a semaphore and on top request based on thread unique 3 digit a thread is picked for operation

-> Operations of read and write is  also secured by semaphores.

->Reader can lock the writer while reading, as no. of readers reaches zero and writer gets unlocked and its request can continue.  
---------------------------The END-------------------------------
