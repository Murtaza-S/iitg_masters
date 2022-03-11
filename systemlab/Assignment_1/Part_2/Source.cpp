/*
RolL No :	214101003
			214101031
Question 2 Solution

"make" command to run.

USE README.

*/



#include<iostream>
#include<thread>
#include <random>
#include<unordered_set>
//#include<mutex>
#include<semaphore>
#include <queue> 

//std::mutex wr_m; // writers mutex
//std::mutex re_m; // readers mutex
//std::mutex re_cnt_m; // readers count mutex
//std::mutex ord_m; // Access Order mutex 

class Comparator //compare first of pair in pq

{
public:
	bool operator()(const std::pair<int, std::pair<int, int>>& left, const std::pair<int, std::pair<int, int>>& right) { return left.first > right.first; }
};

std::counting_semaphore<1> wr_s{1}; //writers semaphore
std::counting_semaphore<1> rd_s{1}; //readers semaphore
std::counting_semaphore<1> rd_cnt_s{1}; //readers count semaphore
std::counting_semaphore<1> ord_s{1}; // semaphore for the priority queue to maintain order


int no_of_friends_reading = 0; // count of current no. of readers
auto ops_q = std::priority_queue <std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>, Comparator>();
int count_q=0;


void thread_manager(int ,int); //each thread randomly selects and operation to execute (wr or rd) and request to enqueue that operation
void operation_queue(int, int, int); //enqueues and dequeues operation from a priority queue
void write_to_card(int ); //Writer function
void read_from_card(int ); //Reader function
void init_order(int *,int); //Randomly assign 3 digit unique code to each thread




int main(void)
{
	int guest_count = 0;
	std::cout << "Please enter the no. of guests: ";
	std::cin >> guest_count;
	

	std::thread* guests = new std::thread[guest_count]; //A Thread for each guest
	int *guest_order = new int[guest_count]; //Array to store unique code for each guest



	init_order(guest_order, guest_count); //Assign Unique code

	/*for (int i = 0; i < guest_count; i++)
	{
		std::cout << *(guest_order + i) << std::endl;
	}*/

	for (int i = 0; i < guest_count; i++)
	{
		*(guests+i) =  std::thread(thread_manager,i,*(guest_order+i)); //Call thread execution
	}

	for (int i = 0; i < guest_count; i++)
	{
		(guests + i)->join();   //thread join
	}


	return 0;
}

void init_order(int* guest_order, int guest_count)
{
	std::unordered_set<int> ord_list; 
	std::srand(time(0));   //seed random function

	int temp_num = 0;
	for (int i = 0; i<guest_count; ) 
	{
		temp_num = rand() % 1000;
		while (ord_list.find(temp_num) != ord_list.end()) // check if code already in set
		{
			temp_num = rand() % 1000;	// set a new code 
		}
		if (temp_num > 100)
		{
			ord_list.insert(temp_num);
			*(guest_order + i) = temp_num; //assign code to thread
			i++;
		}

	}

}


void thread_manager(int thread_index, int code)
{
	std::srand(time(NULL));
	int temp_num,i=0;

	while (true) // inifinte loop
	{
		temp_num = rand()+code;  //random no. for operation selection and wait after request
		operation_queue(thread_index+1, code , temp_num % 2); //Enqueue operation to priority queue
		std::this_thread::sleep_for(std::chrono::microseconds(temp_num%100)); // wait for making a request between 1 to 100 micro sec
	}
	
}

void operation_queue(int thread_index, int code, int op_code)
{

	ord_s.acquire(); //Lock the priorty queue to maintain order
	count_q++;// Operations count
	ops_q.push(std::make_pair(code, std::make_pair(thread_index, op_code))); //priority queue base on code
	ord_s.release(); //Unlock

	std::this_thread::sleep_for(std::chrono::microseconds(120)); //back-off time after request 

	ord_s.acquire(); //Lock to dequeue from priority quee
	std::pair<int, std::pair<int, int>> operation = ops_q.top();

	ops_q.pop();
	count_q--; // Operations count 
	ord_s.release(); //Unlock


	if (operation.second.second == 0 ) //Select operation base on request
	{
		write_to_card(operation.second.first);
	}
	else
	{
		read_from_card(operation.second.first);
	}
}


void write_to_card(int thread_index)
{
	rd_s.acquire(); //Lock readers
	wr_s.acquire(); //Lock writers
	std::cout << "Friend:" << thread_index << " is writing on the card!" << std::endl;
	rd_s.release(); //unlock readers
	wr_s.release(); //unlock writers
}


void read_from_card(int thread_index)
{
	
	rd_s.acquire(); //lock readers
	rd_cnt_s.acquire(); // lock to update readers_count
	no_of_friends_reading++;
	if (no_of_friends_reading == 1)
	{
		wr_s.acquire(); //Lock writers
	}
	rd_cnt_s.release(); //unlock readers
	
	
	std::cout << "Friend:" << thread_index<< " is reading from the card!" <<std::endl;
	rd_s.release(); //unlock readers

	rd_cnt_s.acquire(); // lock update to no. of readers
	no_of_friends_reading--;
	if (no_of_friends_reading == 0 )
	{
		wr_s.release(); //unlock writera
	}
	
	rd_cnt_s.release();//unlock readers_count
}