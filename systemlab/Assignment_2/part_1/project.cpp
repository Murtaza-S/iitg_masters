#include <iostream>
#include<stdlib.h>
#include<fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

#define INPUT_FILE "input.txt"

using namespace std;


//capturing hotel information from file
int hotel_info(vector<int>& rooms, vector<int>& requests);

//findinf the optimal to compare
vector<int> find_optimal(vector<int> rooms, vector<int> requests);

//manager function handling all the managers
void manager_allot(vector<int> rooms, vector<int> requests);

//Manager 1 function returning stats : Fastest fit ( Next fit)
vector<int> aman_room_allot(vector<int> rooms, vector<int> requests);

//Manager 2 function returning stats : Worst fit
vector<int> raj_room_allot(vector<int> rooms, vector<int> requests);

//Manager 2 function returning stats : Best fit
vector<int> alok_room_allot(vector<int> rooms, vector<int> requests);

//Printing the Manager's performance
void print_manager_performance(vector<int> opt_occupency,vector<int> aman_stats,vector<int> raj_stats,vector<int> alok_stats);

int main(){

    //Declaration

    //Vector used to store the requests
    //vector of size 3 to get room numbers. 
    vector<int> requests, rooms (4, 0);

    //Reading of the file 

    //function call to read the file and get these back
    if(hotel_info(rooms, requests))
        return 0;
    


    
    

    //Calling the managers functions 

    manager_allot(rooms, requests);

    //Calculate the difference from optimal 

}

int hotel_info(vector<int>& rooms, vector<int>& requests){

    //Open the file
    fstream fout(INPUT_FILE);

    //Catch if file not Opened
    if(!fout){
        cout<<"File Wasn't opened."<<endl;
        exit(0);
    }

    string str, num;
    getline(fout, str);
    stringstream alpha(str);

    getline(alpha, num, ' ');
    rooms[1] = stoi(num);

    getline(alpha, num, ' ');
    rooms[2] = stoi(num);

    getline(alpha, num, ' ');
    rooms[3] = stoi(num);

    getline(fout, str);
    int total_req = stoi(str);

    getline(fout, str);
    stringstream alpha1(str);

    for(int i = 0; i < total_req ; ++i){
        getline(alpha1, num, ' ');
        // cout<<num<<endl;
        if(num == "S")
        requests.push_back(1);
        else if(num == "D")
        requests.push_back(2);
        else if(num == "T")
        requests.push_back(3);
        else{
            cout<<"Wrong input to room request."<<endl;
            return 1;
        }
    }

    return 0;

}


vector<int> find_optimal(vector<int> rooms, vector<int> requests){

    
    vector<int> opt_occupency = rooms;
    vector<int> room_copy = rooms;
    unordered_map<int, int> mp;

    //flags
    int flag3 = 0, flag2 = 0, flag1 = 0;

    //Mapping the request of rooms to find the optimal
    for(auto req : requests){
        mp[req]++;
    }

    // cout<<mp[3]<<" "<<mp[2]<<" "<<mp[1]<<endl; 

    //Allocating the optimal rooms

    //Allocating triple sharing first
    rooms[3] > mp[3] ? room_copy[3] -= mp[3] : ((room_copy[3] = 0) && (flag3 = 1));

    // cout<<"step "<<room_copy[3]<<" "<<room_copy[2]<<endl;

    //Allocating double sharing 
    //checking if triple are full or empty 
    if(flag3==1){
        rooms[2] > mp[2] ? room_copy[2] -= mp[2] : ((room_copy[2] = 0) && (flag2 = 1));
    }
    else{
        room_copy[3] > mp[2] ? (room_copy[3] -= mp[2]) && (mp[2] = 0) : ( (mp[2] -= room_copy[3]) && (room_copy[3] = 0) && (flag3 = 1) );
        rooms[2] > mp[2] ? room_copy[2] -= mp[2] : ( (room_copy[2] = 0) && (flag2 = 1) );
    }

    //Allocating single sharing room
    //checking if triple or double sharing is available or not
    if(flag3 == 1){
        if(flag2 == 1){
            //Both triple and double arn't there
            //allocate in single sharing
            room_copy[1] > mp[1] ? room_copy[1] -= mp[1] : ((room_copy[1] = 0) && (flag1 = 1));
        }
        else{
            //No triple, Double available
            room_copy[2] > mp[1] ? (room_copy[2] -= mp[1]) && (mp[1] = 0) : ((mp[1] -= room_copy[2]) && (room_copy[2] = 0) && (flag2 = 1) );
            rooms[1] > mp[1] ? room_copy[1] -= mp[1] : ((room_copy[1] = 0) && (flag1 = 1));
        }
    }

    else{
        //Triple rooms available
        room_copy[3] > mp[1] ? (room_copy[3] -= mp[1]) && (mp[1] = 0) : ((mp[1] -= room_copy[3]) && (room_copy[3] = 0) && (flag3 = 1) );
        if(mp[1]){
            if(flag2 == 1){
                //Triple allocated only single remain.
                rooms[1] > mp[1] ? room_copy[1] -= mp[1] : ((room_copy[1] = 0) && (flag1 = 1));
            }
            else{
                //Double remain as well
                room_copy[2] > mp[1] ? (room_copy[2] -= mp[1]) && (mp[1] = 0) : ((mp[1] -= room_copy[2]) && (room_copy[2] = 0) && (flag2 = 1) );
                rooms[1] > mp[1] ? room_copy[1] -= mp[1] : ((room_copy[1] = 0) && (flag1 = 1));
            }
        }
    }

    opt_occupency[3] -= room_copy[3]; 
    opt_occupency[2] -= room_copy[2]; 
    opt_occupency[1] -= room_copy[1];

    return opt_occupency; 


}

void manager_allot(vector<int> rooms, vector<int> requests){

    //Calculate the optimal function 
    // cout<<"req"<<requests[0]<<endl;

    vector<int> opt_occupency = find_optimal(rooms, requests);
    
    // cout<<opt_occupency[3]<<" "<<opt_occupency[2]<<" "<<opt_occupency[1]<<endl;

    

    //Manager 1 call
    vector<int> aman_stats = aman_room_allot(rooms, requests);

    //Manager 2 call
    vector<int> raj_stats = raj_room_allot(rooms, requests);

    //Manager 3 call
    vector<int> alok_stats = alok_room_allot(rooms, requests);

    //Calling the print function to Output the Performance of each.
    print_manager_performance(opt_occupency, aman_stats, raj_stats, alok_stats);


}

vector<int> aman_room_allot(vector<int> rooms, vector<int> requests){

    vector<int> aman_stats(4,0);
    vector<int> room_copy = rooms;

    //Follows the order of fastest fit first thus going : Triple -> Signle -> Double
    for(int req : requests){



        //Checking if can fit is triple
        if(room_copy[3] > 0){
            //Adding the request that is allocated.
            aman_stats[req]++;
            //Removing a triple room from the list.
            room_copy[3]--;

            // cout<<"3 :" <<req<<endl;
            continue;

            
        }

        //Checking is it can be accomodated in single sharing
        if(req == 1){

            if(room_copy[1] > 0){
                //Adding the request that is allocated.
                aman_stats[req]++;
                //Removing a single room.
                room_copy[1]--;

                // cout<<"1 :" <<req<<endl;
                continue;
            }
            
        }

        //Lastly: If double can be accomodated.
        if(req < 3){

            if(room_copy[2] > 0){
                //Adding the request to the stats.
                aman_stats[req]++;
                //Removing a double room.
                room_copy[2]--;

                // cout<<"2 :" <<req<<endl;
            }
        }

    }

    //Checking 
    // cout<<"Aman : "<<aman_stats[1]<<" "<<aman_stats[2]<<" "<<aman_stats[3]<<endl;
    // cout<<aman_stats[1]<<endl;


    return aman_stats;
}

vector<int> raj_room_allot(vector<int> rooms, vector<int> requests){

    vector<int> raj_stats(4,0);
    vector<int> room_copy = rooms;

    //Follows the order of worst fit thus going : Triple -> Double -> Signle
    for(int req : requests){



        //Checking if can fit is triple
        if(room_copy[3] > 0){
            //Adding the request that is allocated.
            raj_stats[req]++;
            //Removing a triple room from the list.
            room_copy[3]--;

            // cout<<"3 :" <<req<<endl;
            continue;

            
        }

        //Checking is it can be accomodated in double sharing
        if(room_copy[2] > 0 && req < 3){
                //Adding the request that is allocated.
                raj_stats[req]++;
                //Removing a single room.
                room_copy[2]--;

                // cout<<"2 :" <<req<<endl;
                continue;
            
            
        }

        //Lastly: If double can be accomodated.
        if(req == 1 && room_copy[1] > 0){

                //Adding the request to the stats.
                raj_stats[req]++;
                //Removing a double room.
                room_copy[1]--;

                // cout<<"1 :" <<req<<endl;
            
        }

    }

    //Checking 
    // cout<<"raj : "<<raj_stats[1]<<" "<<raj_stats[2]<<" "<<raj_stats[3]<<endl;
    // cout<<raj_stats[1]<<endl;


    return raj_stats;
}

vector<int> alok_room_allot(vector<int> rooms, vector<int> requests){

    vector<int> alok_stats(4,0);
    vector<int> room_copy = rooms;

    // cout<<"Req : ";
    // for(auto req : requests)
    //     cout<<req<<" ";
    // cout<<endl;

    //Follows the order of Best fit.
    for(int req : requests){
        // cout<<req<<" ";

        //Firstly: If single request can be accomodated.
        if(req == 1 && room_copy[1] > 0){

                //Adding the request to the stats.
                alok_stats[req]++;
                //Removing a double room.
                room_copy[1]--;

                // cout<<"1 :" <<req<<endl;
                continue; 
        }

        //Checking is it can be accomodated in double sharing
        if(room_copy[2] > 0 && req < 3){
                //Adding the request that is allocated.
                alok_stats[req]++;
                //Removing a single room.
                room_copy[2]--;

                // cout<<"2 :" <<req<<endl;
                continue;
            
            
        }

        //Lastly : Checking if triple are available
        if(room_copy[3] > 0){
            //Adding the request that is allocated.
            alok_stats[req]++;
            //Removing a triple room from the list.
            room_copy[3]--;

            // cout<<"3 :" <<req<<endl;
            continue;

            
        }

        

        

    }

    //Checking 
    // cout<<"alok : "<<alok_stats[1]<<" "<<alok_stats[2]<<" "<<alok_stats[3]<<endl;
    // cout<<alok_stats[1]<<endl;


    return alok_stats;
}

void print_manager_performance(vector<int> opt_occupency,vector<int> aman_stats,vector<int> raj_stats,vector<int> alok_stats){

    //create an array for individual performance
    vector<int> aman(4, 0), raj(4, 0), alok(4, 0);

    //Calculate the best performer.
    
    //Calculating the Revenue generated
    aman[0] = aman_stats[1]*5000 + aman_stats[2]*9000 + aman_stats[3]*12500;
    raj[0] = raj_stats[1]*5000 + raj_stats[2]*9000 + raj_stats[3]*12500;
    alok[0] = alok_stats[1]*5000 + alok_stats[2]*9000 + alok_stats[3]*12500;    
    
    
    //Revenue Wasted 

    aman[1] = (opt_occupency[1] - aman_stats[1]) * 5000 + (opt_occupency[2] - aman_stats[2]) * 9000 + (opt_occupency[3] - aman_stats[3]) * 12500;    
    raj[1] = (opt_occupency[1] - raj_stats[1]) * 5000 + (opt_occupency[2] - raj_stats[2]) * 9000 + (opt_occupency[3] - raj_stats[3]) * 12500;    
    alok[1] = (opt_occupency[1] - alok_stats[1]) * 5000 + (opt_occupency[2] - alok_stats[2]) * 9000 + (opt_occupency[3] - alok_stats[3]) * 12500;    

    //Booking cancelled from optimal booking
    aman[2] = (opt_occupency[1] - aman_stats[1]) + (opt_occupency[2] - aman_stats[2]) + (opt_occupency[3] - aman_stats[3]);
    raj[2] = (opt_occupency[1] - raj_stats[1]) + (opt_occupency[2] - raj_stats[2]) + (opt_occupency[3] - raj_stats[3]);
    alok[2] = (opt_occupency[1] - alok_stats[1]) + (opt_occupency[2] - alok_stats[2]) + (opt_occupency[3] - alok_stats[3]);

    //Best Performer
    if(aman[0] >= raj[0]){
        if(aman[0] > raj[0]){
            (aman[0] > alok[0]) ? (aman[3] = 1) : ( (aman[0] == alok[0]) ? ( (aman[3] = 1) && (alok[3] = 1) ) : (alok[3] = 1) ) ;
        }
        else
            (aman[0] > alok[0]) ? ( (aman[3] = 1) && (raj[3] = 1) ) : ( (aman[0] == alok[0]) ? ( (aman[3] = 1) && (raj[3] = 1) && (alok[3] = 1) ) : (alok[3] = 1) ) ;
    }

    else{
        (raj[0] > alok[0]) ? (raj[3] = 1) : ( (raj[0] == alok[0]) ? ( (raj[3] = 1) && (alok[3] = 1) ) : (alok[3] = 1)) ;
    }

    //Print the table of individual performance
    // cout<<aman[3] << "\t" << raj[3] << "\t" << alok[3] << endl;

    cout<<" Name : "<< "\t" << "Revenue Generated" << "\t" << " Revenue Lost" <<"\t"<< "Booking's Lost" <<"\t" << "Best Performer" << endl;
    cout<<" Aman : "<< "\t" << aman[0] << "\t\t\t\t" << aman[1] << "\t\t" << aman[2] << "\t\t" << aman[3] << endl;
    cout<<" Raj  : "<< "\t" << raj[0] << "\t\t\t\t" << raj[1] << "\t\t" << raj[2] << "\t\t" << raj[3] << endl;
    cout<<" Alok : "<< "\t" << alok[0] << "\t\t\t\t" << alok[1] << "\t\t" << alok[2] << "\t\t" << alok[3] << endl;
    

    return;

}