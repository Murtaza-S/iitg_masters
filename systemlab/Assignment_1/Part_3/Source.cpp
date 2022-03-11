/*
RolL No :	214101003
			214101031
Question 3 Solution

"make" command to run.

USE README.

*/

#include<iostream>
#include<stdlib.h>
#include<fstream>
#include <sstream>
#include<thread>
#include<mutex>

#define MAX_SIZE 100

using namespace std;

mutex locking;
int turn = 0;

void resource_allocation(int id, int sizes[2], int safe_sequence[MAX_SIZE], int pre_allocated[MAX_SIZE], int need_allocation[MAX_SIZE], int released_resources[MAX_SIZE][MAX_SIZE]);

//Initializing the Matrices from the File .
void file_matrix_initialization(int sizes[2], int available[MAX_SIZE], int pre_allocated[MAX_SIZE][MAX_SIZE], int max_aloacation[MAX_SIZE][MAX_SIZE], int need_allocation[MAX_SIZE][MAX_SIZE], int released_resources[MAX_SIZE][MAX_SIZE]) ;




int main(){


    //Initialization of the variables 
    int pre_allocated[MAX_SIZE][MAX_SIZE]= {0};
    int max_aloacation[MAX_SIZE][MAX_SIZE]= {0};
    int need_allocation[MAX_SIZE][MAX_SIZE] = {0};
    int available[MAX_SIZE] = {0};
    int released_resources[MAX_SIZE][MAX_SIZE] = {-100};
    int sizes[2]={0}; //(0,1) to store no of processes and no of resources;



    //Initializing the Matrices from the File .
    // file_matrix_initialization();
    file_matrix_initialization( sizes , available,  pre_allocated,  max_aloacation,  need_allocation,released_resources) ;

    


    //Variable declaration for finding safe sequence

    int proc = 0; //Iterator for safe sequence
    int i = 0; //Iterator
    int safe_sequence[MAX_SIZE] = {0}; //To load safe sequence

    bool flag = true; // to check if any process in current sequence is processed or not to exit the loop.

    int exec_process[MAX_SIZE] = {0};
    
    while(1){

        if(i == 0){
            if (flag == false)
            break;
            else 
            flag = false;

        }

        //Check if the process is executed;
        // If it is get to next process
        if(exec_process[i]){
            // cout<<endl<<"Proc skipped"<<i;
            i = ++i % sizes[0];
            continue;
        }

        //Check if the process can be executed with availale resources

        exec_process[i] = 1;
        for(int q = 0; q < sizes[1]; ++q){
            //run loop to check each resource can be allocated
            //If allocation fails break from current loop.
            if (available[q] < need_allocation [i][q]){
                // cout<<endl<<"Can't be allacated."<<i;
                exec_process[i] = 0;
                // cout<<endl<<i<<exec_process[i];
                break;
            }
        }

        //if last resource got allocated
        // cout<<endl<<"EP"<<i<<exec_process[i];
        if(exec_process[i]){
            //Add the previous allocated resources back to available
            //Add the process into the safe sequence
            //Increment the sequence iterator 

            //Adding the process to the next position in safe sequence
            safe_sequence[proc] = i;
            // cout<<endl<<"Is Allocated";
            for(int q = 0; q <= proc; ++q){
                // cout<<safe_sequence[q]<<" ";
                }

            //Releasing the Pre Allocated resources to available
            for(int q = 0; q < sizes[1]; ++q){
                available[q] += pre_allocated[i][q];
                released_resources[proc+1][q] = available[q];
            }

            proc++;
            flag = true;
        }

        //To keep the process iteartor working 
        i = ++i % sizes[0];

    }

    //if the sequence iterator is less than the n-1 
    //Implies that the loop got stopped in middle as no process got executed
    //Lable : No safe sequence
    if (proc < sizes[0]){
        cout<<"No safe sequence found for the processes.";
        return 0;
    }

    //Printing the Safe Sequence
    cout<< "Safe Sequence for Process Allocation : ";

    for(int q = 0; q < sizes[0]; ++q){
        cout<<safe_sequence[q]<<" ";
    }

    

    //If not safe sequence found.
    //Create the threads FOR THE OUTPUT
    thread threads[sizes[0]];


    //Resource Allocationg a.k.a. Printing form the threads.
    for(int q = 0; q < sizes[0]; ++q){
        threads[q] = thread(resource_allocation, q, sizes, safe_sequence, pre_allocated[safe_sequence[q]], need_allocation[safe_sequence[q]],released_resources);
    }

    for(int q = 0; q < sizes[0]; ++q){
        threads[q].join();
    }

    cout<<endl<<endl<<"All the Threads have finished processing. Alpha Mike Foxtrot";

    return 0;


    // Keep the threads in busy wait till according to their precedence in safe sequence no is up
    //Print the message and join with the main again and wait for all the others to get executed 

    //Closing the file after reading
}

void resource_allocation(int id, int sizes[2], int safe_sequence[MAX_SIZE], int pre_allocated[MAX_SIZE], int need_allocation[MAX_SIZE], int released_resources[MAX_SIZE][MAX_SIZE]){
    
    while(id != turn);

    locking.lock();

    cout<<endl<<endl;
    cout<<endl<<"--> Process "<<safe_sequence[id]+1;
    cout<<endl<<'\t'<<"Allocated : ";
    for(int q = 0; q < sizes[1]; ++q){
        cout<<pre_allocated[q]<<" ";
    }

    cout<<endl<<'\t'<<"Needed    : ";
    for(int q = 0; q < sizes[1]; ++q){
        cout<<need_allocation[q]<<" ";
    }

    cout<<endl<<'\t'<<"Available : ";
    for(int q = 0; q < sizes[1]; ++q){
        cout<<released_resources[id][q]<<" ";
    }

    cout<<endl<<'\t'<<"Resource Allocated!";
    cout<<endl<<'\t'<<"Process Code Running..";
    cout<<endl<<'\t'<<"Process Code Completed";
    cout<<endl<<'\t'<<"Process Releasing Resource..";
    cout<<endl<<'\t'<<"Resource Released!";
    cout<<endl<<'\t'<<"Now Available : ";
    for(int q = 0; q < sizes[1]; ++q){
        cout<<released_resources[id+1][q]<<" ";
    }

    turn++;

    locking.unlock();

    return;
}

void file_matrix_initialization(int sizes[2], int available[MAX_SIZE], int pre_allocated[MAX_SIZE][MAX_SIZE], int max_aloacation[MAX_SIZE][MAX_SIZE], int need_allocation[MAX_SIZE][MAX_SIZE], int released_resources[MAX_SIZE][MAX_SIZE]){
    //Reading the file to get the data
    
    //Open the file
    fstream fout("input.txt");

    //Catch if file not Opened
    if(!fout){
        cout<<"File Wasn't opened.";
        exit(0);
    }
    string str, num;
    getline(fout, str);
    stringstream alpha(str);

    getline(alpha, num, ' ');
    sizes[0] = stoi(num);

    getline(alpha, num, ' ');
    sizes[1] = stoi(num);

    getline(fout, str);
    stringstream alpha1(str);
    for (int q = 0 ; q < sizes[1]; ++q){

        getline(alpha1, num, ' ');
        available[q] = stoi(num);
        // cout<<available[q]<<" ";
        released_resources[0][q] = available[q];

    }


    //Reading the Pre Allocated Matrix
    for(int w = 0; w < sizes[0]; ++w){
        // cout<<endl;
        getline(fout, str);
        stringstream alpha1(str);
        
        for (int q = 0 ; q < sizes[1]; ++q){

            getline(alpha1, num, ' ');
            //cout<<num<<" ";
            pre_allocated[w][q] = stoi(num);
            // cout<<pre_allocated[w][q]<<" ";

        }

    }

    //Reading the Max Allocation Matrix
    for(int w = 0; w < sizes[0]; ++w){

        // cout<<endl;

        getline(fout, str);
        stringstream alpha1(str);
        
        for (int q = 0 ; q < sizes[1]; ++q){

            getline(alpha1, num, ' ');
            //cout<<num<<" ";
            max_aloacation[w][q] = stoi(num);
            // cout<<max_aloacation[w][q]<<" ";

        }

    }
    //Closing the file
    fout.close();

    //Calculating the Need Allocation Matrix

    for(int w = 0; w < sizes[0]; ++w){

        // cout<<endl;
        
        for (int q = 0 ; q < sizes[1]; ++q){

            need_allocation[w][q] = max_aloacation[w][q] - pre_allocated[w][q];
            // cout<<max_aloacation[w][q] - pre_allocated[w][q]<<" ";

            // cout<<need_allocation[w][q]<<" ";

        }

    }


    // cout << "\nMatrix A : " << sizes[0] << " x " << sizes[1] << "\n";
}
