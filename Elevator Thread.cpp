#include <iostream>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <chrono>

std::mutex mtx;                  
int current_floor = 1;           
const int NUM_FLOORS = 9;       

// elevator moving
void move_to_floor(int requested_floor) {
    mtx.lock();
    std::cout << "Elevator moving from " << current_floor << " to " << requested_floor << "...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(std::abs(current_floor - requested_floor) * 500));
    current_floor = requested_floor;
    std::cout << "Elevator has arrived at floor " << current_floor << ".\n";
    mtx.unlock();
}

// giving a random floor number
int get_random_floor() {
    return 1 + rand() % NUM_FLOORS; 
}

// giving a floor to the user
void request_simulation(int user_id) {
    int requested_floor = get_random_floor(); 
    std::cout << "User " << user_id << " is requesting floor " << requested_floor << "...\n";
    move_to_floor(requested_floor);
}

int main() {
    std::cout << "Elevator Simulation Starting...\n";

    // gives random number w unique seed of floor
    srand(time(0));

    // user pressing button
    std::thread user1(request_simulation, 1); 
    std::thread user2(request_simulation, 2); 
    std::thread user3(request_simulation, 3); 

   
    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "Simulation ended.\n";

    // exit
    user1.detach();
    user2.detach();
    user3.detach();

    return 0;
}
