/*
RolL No :	214101003
			214101031
Question 1 Solution

"make" command to run.

USE README.

*/

#include<iostream>
#include<fstream>
#include<sstream>
#include<queue>
#include<string>
#include<list>
#include<random>

#define FILE_NAME "arrival.txt"

using namespace std;


class Comparator //compare first of pair in pq

{
public:
	bool operator()(const pair<int, int>& left, const pair<int, int>& right)
	{
		if (left.second > right.second) return true;
		if (left.second < right.second) return false;
		return left.first > right.first;
	}
	};

class PSRT
{
	vector<pair<int, int>> proc_des;
	priority_queue<pair<int, int>, vector<pair<int, int>>, Comparator> remain_time;
	vector<int> finish_time;
	list<pair<int, pair<int, int>>> ar_time;
	list<pair<int, int>> gantt_chart;
	int proc_count = 0;
	int total_bust = 0;

public:
	void init(string arrivals);
	void process();
	void print_chart();

};

void PSRT::init(string arrivals)
{
	srand(time(NULL));

	int temp_bust = 0;
	int temp_arrival = 0;
	
	istringstream arv_times(arrivals);
	string time;
	while (getline(arv_times, time, ' '))
	{

		temp_bust = (rand() % 8) + 1;
		temp_arrival = stoi(time);
		
		ar_time.push_back(make_pair(temp_arrival,make_pair(proc_count, temp_bust)));
		proc_count++;
		total_bust += temp_bust;
	}
	proc_des = vector<pair<int, int>>(proc_count);
	finish_time = vector<int>(proc_count,-1);

	auto temp_iterator = ar_time.begin();
	for (int i = 0; i < proc_count; i++)
	{
		proc_des[i] = make_pair((temp_iterator)->first,(temp_iterator)->second.second);
		//remain_time.push(make_pair(i, (temp_iterator++)->second.second));
		temp_iterator++;
	}

	ar_time.sort();
}

void PSRT::process()
{
	int curr_p = -1;
	auto temp_iterator = ar_time.begin();
	int first_proc_ar_t = temp_iterator->first;

	for (int i = 0; i < first_proc_ar_t+total_bust; i++)
	{
		
		while (ar_time.end() != temp_iterator && temp_iterator->first <= i)
		{
			if (finish_time[(temp_iterator)->second.first] == -1)
			{
				remain_time.push(make_pair((temp_iterator)->second.first, (temp_iterator)->second.second));
			}

			temp_iterator++;
		}
		
		if (!remain_time.empty())
		{
			auto curr_proc_desc = remain_time.top();
			curr_p = curr_proc_desc.first;
			remain_time.pop();

			gantt_chart.push_back(make_pair(curr_proc_desc.first, curr_proc_desc.second));

			curr_proc_desc.second -= 1;
			if (curr_proc_desc.second != 0)
			{
				remain_time.push(curr_proc_desc);
			}
			else
			{
				finish_time[curr_p] = i+1;
			}
		}
	}
}


//Printing of the Gantt Chart 
void PSRT::print_chart()
{

	//Printing the Process Information : Arrival, Time needed, Wait, and Finish time 
	cout << endl << "Process Num  : ";
	for (int i = 0; i < proc_count; i++)
	{
		cout << "p" << i << " ";
	}
	cout << endl << "Arrival time : ";
	for (int i = 0; i < proc_count; i++)
	{
		//cout << proc_des[i].first << " ";
		printf("%2d ", proc_des[i].first);
	}
	cout << endl << "Cook time    : ";
	for (int i = 0; i < proc_count; i++)
	{
		//cout << proc_des[i].second << " ";
		printf("%2d ", proc_des[i].second);
	}
	cout << endl << "Finish time  : ";
	for (int i = 0; i < proc_count; i++)
	{
		//cout << finish_time[i] << " ";
		printf("%2d ", finish_time[i]);
	}

	cout << endl << "Wait time    : ";
	for (int i = 0; i < proc_count; i++)
	{
		//cout << finish_time[i] << " ";
		printf("%2d ", finish_time[i]- proc_des[i].second - proc_des[i].first);
	}


	// //Printing the Gantt Chart with Process number ad 
	// cout << endl << "Gantt Chart [Time: Pr_No. - Remain time]: " << endl;
	// int index_time = 0;
	// for (auto it = gantt_chart.begin(); it != gantt_chart.end(); it++)
	// {
	// 	printf("[ %d: p%d - %2d ]", index_time++ ,(it)->first,  (it)->second);
		
	// }
	cout << endl << "\nGantt Chart [Process Number]: " << endl;
	
	for (auto it = gantt_chart.begin(); it != gantt_chart.end(); it++)
	{
		printf("[p%d]",  (it)->first);

	}
	cout << endl << endl;
}



int main(void)
{
	auto file_name = FILE_NAME;
	ifstream infile;
	infile.open(file_name);
	string fileline;
	PSRT* myRestaurant;

	while (getline(infile, fileline))
	{
		myRestaurant = new PSRT();
		myRestaurant->init(fileline);
		myRestaurant->process();
		myRestaurant->print_chart();

		delete myRestaurant;
	}

	return 0;
}