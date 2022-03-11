#include<iostream>
#include<stdlib.h>
#include<fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <typeinfo>

#define BRANCHES 3

using namespace std;

//Reading file for library details
int library_records(vector<int> &subjects, vector<int> &tray, vector<string> &requests);

void librarian(vector<int> subjects, vector<int> tray, vector<string> requests);

void btech_tray_fifo(int tray_size, string req, vector<string> &bt_tray , unordered_map<string, int> &bt_lookup, int &misses);

void mtech_tray_lfu(int tray_size, string req, vector<string> &mt_tray, unordered_map<string, int> &mt_lookup, int &misses);

void phd_tray_lru(int tray_size, string req, vector<string> &phd_tray, unordered_map<string, int> &phd_lookup, int &misses);

void print_branch(vector<string> tray, unordered_map<string, int> lookup, string branch_name);

void print_tray(vector<string> tray);


int main(){

    //Delcaration
    vector<int> subjects(BRANCHES,0);
    vector<int> tray(BRANCHES, 0);
    vector<string>  requests;



    //Reading from the file
    if(library_records(subjects, tray, requests)){
        cout<<"Issue is reading file.";
        return 0;
    }

    

    //Fucntion to handle the requests


    librarian( subjects, tray, requests);



    //for each request call the appropriate function tray 

    //Keep track of the tray only and book no allocated
    
}


int library_records(vector<int> &subjects, vector<int> &tray, vector<string> &requests){

    //Open the file
    fstream fout("t1.txt");

    //Catch if file not Opened
    if(!fout){
        cout<<"File Wasn't opened.";
        return 1;
    }

    //Reading the subjects for each branch
    string str, num;
    getline(fout, str);
    stringstream alpha(str);

    getline(alpha, num, ' ');
    subjects[0] = stoi(num);
    // cout<<subjects[0]<<" ";

    getline(alpha, num, ' ');
    subjects[1] = stoi(num);
    // cout<<subjects[1]<<" ";

    getline(alpha, num, ' ');
    subjects[2] = stoi(num);
    // cout<<subjects[2]<<" "<<endl;

    //Reading the tray sizes for each branch
    getline(fout, str);
    stringstream alpha1(str);

    getline(alpha1, num, ' ');
    tray[0] = stoi(num);
    // cout<<tray[0]<<" ";

    getline(alpha1, num, ' ');
    tray[1] = stoi(num);
    // cout<<tray[1]<<" ";

    getline(alpha1, num, ' ');
    tray[2] = stoi(num);
    // cout<<tray[2]<<" "<<endl;

    while(getline(fout, str)){
        
        requests.push_back(str);
        // cout<<str<<endl;
        fflush(stdin);
    }

    // for(auto ele : requests){
    //     cout<<ele[0]<<typeid(ele[0]).name()<<endl;
    // }
    return 0;

}


void librarian(vector<int> subjects, vector<int> tray, vector<string> requests){

    //Trays in library maintaning the current state.
    vector<string> bt_tray;
    vector<string> mt_tray(tray[1]);
    vector<string> phd_tray(tray[2]);

    //Frequency map for each book issued
    unordered_map<string, int> bt_lookup;
    unordered_map<string, int> mt_lookup;
    unordered_map<string, int> phd_lookup;

    //Keep check of misses in tray.
    int misses = 0;

    for(auto req : requests){
        
        switch(req[0]){
            case 'B' : //Call the respected Btech. Tray function to handle the request
                        btech_tray_fifo(tray[0], req, bt_tray, bt_lookup, misses);
                        // print_tray(bt_tray);
                        break;

            case 'M' : //Call the respective Mtech tray function to handle the request
                        mtech_tray_lfu(tray[1], req, mt_tray, mt_lookup, misses);
                        // print_tray(mt_tray);
                        break;

            case 'P' : //Call the respective PhD tray function to handle the request
                        phd_tray_lru(tray[2], req, phd_tray, phd_lookup, misses);
                        // print_tray(phd_tray);
                        break;

            default  : cout<<"Book not found in library : " << req << endl;
                        break;
        } 

    }

    //Printing the details :

    //No of misses occured during entire day
    cout<<"Misses : "<<misses<<endl<<endl;

    //For B.tech
    print_branch(bt_tray, bt_lookup, "B. Tech");

    //For M. Tech
    print_branch(mt_tray, mt_lookup, "M. Tech");

    //For Phd
    print_branch(phd_tray, phd_lookup, "PhD");


}

void btech_tray_fifo(int tray_size, string req, vector<string> &bt_tray, unordered_map<string, int> &bt_lookup, int &misses){

    //Increase frequency as it is getting issued
    bt_lookup[req]++;
    // cout<<req<< " 99 "<< bt_lookup[req];
    
    for(auto ele : bt_tray){
        if(!ele.compare(req)){
            //No change in tray
            return;
        }
    }

    misses++;

    if(tray_size == bt_tray.size()){
        vector<string>::iterator it = bt_tray.begin();
        bt_tray.erase(it);
    }
    //Insert the copy at end in tray (recent) 
    bt_tray.push_back(req);

    return;
}

void mtech_tray_lfu(int tray_size, string req, vector<string> &mt_tray, unordered_map<string, int> &mt_lookup, int &misses){

    //LFU
    

    //Iterator to track the min frequency.
    vector<string>::iterator it = mt_tray.begin();


    //Search
    for(int i = 0; i < mt_tray.size(); ++i){
        if(!mt_tray[i].compare(req)){
            //Book present in the tray : Only replacing and increasing the frequency.
            
            //Also track the book with least freq if book not found.
            if(mt_lookup.count(*it) > mt_lookup[mt_tray[i]])
                it = mt_tray.begin() + i;

            mt_lookup[req]++;

            return;

        }
    }

    misses++;

    //Delete
    //Removing the least frequent element if tray is full
    if(mt_tray.size() == tray_size){
        mt_tray.erase(it);
    }
    //insert

    mt_tray.push_back(req);



    //Increase frequency as it is getting issued
    mt_lookup[req]++;



    return;
}

void phd_tray_lru(int tray_size, string req, vector<string> &phd_tray, unordered_map<string, int> &phd_lookup, int &misses){

    //LRU
    phd_lookup[req]++;

    //Search .. If present add it again at back(top). 

    for(int i = 0; i < phd_tray.size(); ++i){
        if(!phd_tray[i].compare(req)){
            phd_tray.erase(phd_tray.begin() + i);
            phd_tray.push_back(req);

            //Returning 
            return;
        }
    }

    misses++;

    //Delete the first element if list full 
    if(phd_tray.size() == tray_size){
    vector<string>::iterator it = phd_tray.begin();
    phd_tray.erase(it);
    }

    //Insert the book in the tray.
    phd_tray.push_back(req);

    return;


}

void print_branch(vector<string> tray, unordered_map<string, int> lookup, string branch_name){

    //ID's which were issued the maximum.
    vector<string> max_el;
    int max_val = 0;

    //For Branch
    fflush(stdout);
    cout<<branch_name<< " tray highest issue : " <<endl;

    for(auto ele: lookup){
        // cout<<lookup[ele];
        if(max_val < ele.second){
            max_el.clear();
            max_el.push_back(ele.first);
            // cout<<ele.first;
            max_val = ele.second;
        }
        else if(max_val == ele.second){
            max_el.push_back(ele.first);
        }

        // cout<<ele.first<<" 00 "<<ele.second<<endl;
    }
    for(int i = 0; i < max_el.size(); ++i)
        cout<<max_el[i]<<endl;
    cout<<endl;

    cout<<branch_name<< " Tray : " <<endl;
    print_tray(tray);

    cout<<endl<<endl;

}

void print_tray(vector<string> tray){
    for(auto ele : tray){
        cout<<ele<<endl;
    }
}
