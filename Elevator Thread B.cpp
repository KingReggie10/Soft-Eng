#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <chrono>

std::mutex mtx;                  
int current_floor = 1;           
const int NUM_FLOORS = 9;       
std::vector<int> requests;      

// Elevator moving
void move_to_floor(int requested_floor) {
    mtx.lock();
    std::cout << "Elevator moving from " << current_floor << " to " << requested_floor << "...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(std::abs(current_floor - requested_floor) * 500));
    current_floor = requested_floor;
    std::cout << "Elevator has arrived at floor " << current_floor << ".\n";
    mtx.unlock();
}

// User floor request simulation
void request_simulation(int user_id) {
    int requested_floor;
    do {
        std::cout << "User " << user_id << ", enter the floor you want to go to (1-" << NUM_FLOORS << "): ";
        std::cin >> requested_floor;

        if (requested_floor < 1 || requested_floor > NUM_FLOORS) {
            std::cout << "Invalid floor number. Please enter a number between 1 and " << NUM_FLOORS << ".\n";
        }
    } while (requested_floor < 1 || requested_floor > NUM_FLOORS);

    mtx.lock();
    requests.push_back(requested_floor);
    mtx.unlock();
}

int main() {
    std::cout << "Elevator Simulation Starting...\n";

    // User input for floor requests
    std::thread user1(request_simulation, 1); 
    std::thread user2(request_simulation, 2); 
    std::thread user3(request_simulation, 3); 

    // Wait for all users to finish
    user1.join();
    user2.join();
    user3.join();

    // Sort requests in ascending order
    mtx.lock();
    std::sort(requests.begin(), requests.end());
    mtx.unlock();

    // Process requests in order
    for (int floor : requests) {
        move_to_floor(floor);
    }

    std::cout << "Simulation ended.\n";

    return 0;
}
