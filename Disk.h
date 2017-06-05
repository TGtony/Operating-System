/**********************
    Tony Gong
    CSCI 340
    OS Project
**********************/

#ifndef DISK_H
#define DISK_H

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "PCB.h"

using namespace std;

struct DiskData
{
    PCB process;
    string file_name;
    string read_or_write;
    int file_size;

    friend ostream &operator<< (ostream &out, const DiskData &disk)
    {
        cout << disk.process << " -> File: " << disk.file_name << ", File Size: "
                << disk.file_size << ", " << disk.read_or_write << ", CPU time: "
                << disk.process.get_cpu_time() << ", Ready time: "
                << disk.process.get_ready_time() << endl;
        return out;
    }
}; // Structure used to hold the PCB and related data for disks

class Disk
{
    public:
        // 1st process in queue uses the disk
        void use_disk()
        {
            current_file.push_back(disk_queue.front());
            disk_queue.pop_front();
        }

        // Process is done accessing disk
        void done_with_disk()
        {
            current_file.pop_back();
        }

        // Put process in disk queue
        void enqueue_file(DiskData file)
        {
            disk_queue.push_back(file);
        }

        // Returns the current disk access process
        DiskData current_disk_access()
        {
            return current_file[0];
        }

        // Checks if disk is idle
        int disk_status()
        {
            return current_file.size();
        }

        // Checks if disk queue is empty
        int disk_queue_size()
        {
            return disk_queue.size();
        }

        // Overload << to cout disks and queues
        // If there's no process currently accessing disk, print idle.
        // If there is a process, print the process.
        // If the queue is not empty, print out every element in queue.
        // If the queue is empty, print queue is empty.
        friend ostream &operator<< (ostream &out, const Disk &rhs)
        {
            if(rhs.current_file.size() == 0)
            {
                cout << "Idle" << endl;
            }
            else
            {
                cout << rhs.current_file[0];
            }

            int counter = 1;
            if(rhs.disk_queue.size() != 0)
            {
                for(list<DiskData>::const_iterator iter = rhs.disk_queue.begin(); iter != rhs.disk_queue.end(); ++iter)
                {
                    cout << counter << ": " << *iter;
                    counter++;
                }
            }
            else
            {
                cout << "Disk queue is empty." << endl;
            }
            return out;
        }

    private:
        vector<DiskData> current_file;
        list<DiskData> disk_queue;
};
#endif
