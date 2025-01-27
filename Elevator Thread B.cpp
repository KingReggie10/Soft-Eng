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


void print_separator() {
std::cout << "----------------------------------------\n";
}

// elevator moving
void move_to_floor(int requested_floor) {
std::lock_guard<std::mutex> lock(mtx);
std::cout << "\n[Elevator] Moving from floor " << current_floor
<< " to floor " << requested_floor << "...\n";
std::this_thread::sleep_for(std::chrono::milliseconds(std::abs(current_floor - requested_floor) * 500));
current_floor = requested_floor;
std::cout << "[Elevator] Arrived at floor " << current_floor << ".\n";
print_separator();
}

// User floor request simulation
void request_simulation(int user_id) {
int requested_floor;
do {
{
std::lock_guard<std::mutex> lock(mtx);
std::cout << "\n[User " << user_id
<< "] Enter the floor you want to go to (1-" << NUM_FLOORS << "): ";
}
std::cin >> requested_floor;

if (requested_floor < 1 || requested_floor > NUM_FLOORS) {
std::lock_guard<std::mutex> lock(mtx);
std::cout << "[Error] Invalid floor number! Please enter a number between 1 and "
<< NUM_FLOORS << ".\n";
}
} while (requested_floor < 1 || requested_floor > NUM_FLOORS);

{
std::lock_guard<std::mutex> lock(mtx);
std::cout << "[User " << user_id << "] Requested floor " << requested_floor << ".\n";
requests.push_back(requested_floor);
}
}

int main() {
std::cout << "=== Welcome to the Elevator Simulation! ===\n";
print_separator();

// user input
std::thread user1(request_simulation, 1);
std::thread user2(request_simulation, 2);
std::thread user3(request_simulation, 3);

// wait for all users to finish
user1.join();
user2.join();
user3.join();

print_separator();
std::cout << "[System] Processing floor requests in order...\n";

// sort requests in ascending order
{
std::lock_guard<std::mutex> lock(mtx);
std::sort(requests.begin(), requests.end());
}

// process requests in order
for (int floor : requests) {
move_to_floor(floor);
}

std::cout << "[System] Simulation has ended. Thank you for using the elevator!\n";
print_separator();

return 0;
}
