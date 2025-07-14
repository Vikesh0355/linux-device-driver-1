#include<iostream>
#include<csignal>
#include<unistd.h>
using namespace std;

void SignalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    // Clean up or gracefully terminate the process if needed
    std::exit(signum);  // Exit the program gracefully

}

int main()
{
    signal(SIGINT, SignalHandler);
   
        // Infinite loop to keep the program running
    std::cout << "Program running. Press Ctrl+C to interrupt.\n";
    while (true) 
    {
        // Simulate some work
        sleep(1);
    }

    return 0;
}

/* Compile:
   g++ linux_signals.cpp -o linux_signals
   Run:
   ./linux_signals
*/