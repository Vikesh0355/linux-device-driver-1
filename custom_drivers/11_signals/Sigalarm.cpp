#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
using namespace std;

// Signal handler for SIGALRM
void alarm_handler(int sig) 
{
    cout<<"Time's up! Alarm triggered!\n"<<endl;
    exit(0);  // Terminate the program
}

int main() {
    // Register signal handler for SIGALRM
    signal(SIGALRM, alarm_handler);

    // Set the alarm to trigger after 5 seconds
    alarm(5);

    // Do some work, here we're just sleeping
    cout<<"Working...\n"<<endl;
    sleep(10);  // Simulate work, but the alarm will interrupt after 5 seconds

    return 0;
}

/* Compile:
   g++ Sigalarm.cpp -o Sigalarm
   Run:
   ./Sigalarm
*/