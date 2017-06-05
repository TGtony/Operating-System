/**********************
    Tony Gong
    CSCI 340
    OS Project
**********************/

#ifndef PCB_H
#define PCB_H

#include <iostream>

using namespace std;

class PCB
{
    public:
        PCB(): pid{0}, process_size{0}{}

        PCB(int mem, int id): pid{id}, process_size{mem}, cpu_time{0}, ready_time{0}{} // Constructor

        // Returns the size of the process
        int get_size()
        {
            return process_size;
        }

        ~PCB() = default;
        // Returns the PID of the process
        int get_PID()
        {
            return pid;
        }

        // Overload << to cout PCB
        friend ostream &operator<< (ostream &out, const PCB &rhs)
        {
            cout << "PID " << rhs.pid;
            return out;
        }

        void increase_cpu_time(int time)
        {
            cpu_time += time;
        }

        void increase_ready_time(int time)
        {
            ready_time += time;
        }

        int get_cpu_time() const
        {
            return cpu_time;
        }

        int get_ready_time() const
        {
            return ready_time;
        }
    private:
        int pid;
        int process_size;
        int cpu_time;
        int ready_time;
};
#endif
