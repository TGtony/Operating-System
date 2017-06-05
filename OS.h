/**********************
    Tony Gong
    CSCI 340
    OS Project
**********************/

#ifndef OS_H
#define OS_H

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include "Printer.h"
#include "Disk.h"
#include "PCB.h"

using namespace std;

struct MemData
{
    int pid;
    int begin1;
    int end1;
    int size1;
    int spaceleft;
    int spaceright;
    int cpu_time;
    int ready_time;
}; // Structure to represent process memory usage

class OS
{
    public:
        // Constructor
        OS(int mem, int numprint, int numdisk)
            : memory{mem}, numberOfPrinters{numprint}, numberOfDisks{numdisk}{};

        // Create printers based on user input during sys gen
        void generate_printers()
        {
            for(int i = 0; i < numberOfPrinters; i++)
            {
                Printer printer;
                printers.push_back(printer);
            }
        }

        // Create disks based on user input during sys gen
        void generate_disks()
        {
            for(int i = 0; i < numberOfDisks; i++)
            {
                Disk hdd;
                disks.push_back(hdd);
            }
        }

        // Assign PID to process
        int pidAssign()
        {
            return pidCounter;
        }

        // Increase counter for next PID
        void increase_next_PID()
        {
            pidCounter++;
        }

        // Put process in ready queue
        void ready_enqueue(PCB process)
        {
            readyqueue.push_back(process);
        }

        // Checks if CPU is idle
        int cpu_status()
        {
            return cpu.size();
        }

        // 1st process in ready queue used CPU
        void enter_cpu()
        {
            PCB process = readyqueue.front();
            cpu.push_back(process);
            readyqueue.pop_front();
        }

        // Allocates memory for process
        void allocate_memory(PCB process)
        {
            MemData newprocess;
            newprocess.pid = process.get_PID();
            newprocess.size1 = process.get_size();
            newprocess.cpu_time = process.get_cpu_time();
            newprocess.ready_time = process.get_ready_time();
            if(memorylocation.empty()) // if memory is empty, place process in memory
            {
                newprocess.begin1 = 0;
                newprocess.end1 = process.get_size()-1;
                newprocess.spaceleft = 0;
                newprocess.spaceright = memory - newprocess.size1;
                memorylocation.push_back(newprocess);

                ready_enqueue(process);
                if(cpu_status() == 0)
                    enter_cpu();
                increase_next_PID();
            }
            else //determine best fit
            {
                int bestfitLtemp;

                int bestfitL=-1;
                int indexL;

                int bestfitRtemp;

                int bestfitR=-1;
                int indexR;

                bool leftorright; // 0 for left, 1 for right
                int indextoinsert;

                int memorylocationsize = memorylocation.size();
                for(int i = 0; i < memorylocationsize; i++) // check best left fit of each process in memory
                {
                    if(memorylocation[i].spaceleft >= newprocess.size1)
                    {
                        bestfitLtemp = memorylocation[i].spaceleft - newprocess.size1;
                        if(bestfitL == -1) // if first check, set as best
                        {
                            bestfitL = bestfitLtemp;
                            indexL = i;
                        }
                        if(bestfitLtemp < bestfitL) // if current fit is better than current best, set this as best
                        {
                            bestfitL = bestfitLtemp;
                            indexL = i;
                        }
                    }
                }

                for(int i = 0; i < memorylocationsize; i++) // check best right fit of each process in memory
                {
                    if(memorylocation[i].spaceright >= newprocess.size1)
                    {
                        bestfitRtemp = memorylocation[i].spaceright - newprocess.size1;
                        if(bestfitR == -1) // if first check, set as best
                        {
                            bestfitR = bestfitRtemp;
                            indexR = i;
                        }
                        if(bestfitRtemp < bestfitR) // if current fit is better than current best, set this as best
                        {
                            bestfitR = bestfitRtemp;
                            indexR = i;
                        }
                    }
                }

                // if no best fit left but there is best fit right, insert to best fit right
                if((bestfitL == -1)&&(bestfitR != -1))
                {
                    leftorright = 1;
                    indextoinsert = indexR;
                }
                // if no best fit right but there is best fit left, insert to best fit left
                else if((bestfitL != -1)&&(bestfitR == -1))
                {
                    leftorright = 0;
                    indextoinsert = indexL;
                }
                // if there's no way to fit this process, no insert
                else if((bestfitL == -1)&&(bestfitR == -1))
                {
                    leftorright = 0;
                    indextoinsert = -1;
                }
                else
                {
                    // if best fit left is better than best fit right, insert to best fit left
                    if(bestfitL < bestfitR)
                    {
                        leftorright = 0;
                        indextoinsert = indexL;
                    }
                    // if best fit right is better than best fit left, insert to best fit right
                    else if(bestfitR < bestfitL)
                    {
                        leftorright = 1;
                        indextoinsert = indexR;
                    }
                    // if both best fits are the same, doesn't matter, I chose to inset to right
                    else
                    {
                        leftorright = 1;
                        indextoinsert = indexR;
                    }
                }

                if(!(indextoinsert < 0)) // if there is a way to insert
                {
                    // inserting to best fit left of a process
                    // update space to left and right of that process
                    // set space to left and right of newly inserted process
                    // update space to left and right of process to left of new inserted process
                    // set where newly inserted process begins and ends
                    if(leftorright==0)
                    {
                        memorylocation.insert(memorylocation.begin() + indextoinsert, newprocess);

                        if(!(indextoinsert-1 < 0))
                            memorylocation[indextoinsert-1].spaceright = 0;

                        memorylocation[indextoinsert+1].spaceleft = memorylocation[indextoinsert+1].spaceleft
                                - newprocess.size1;

                        if(!(indextoinsert-1 < 0))
                            memorylocation[indextoinsert].begin1 = memorylocation[indextoinsert-1].end1+1;
                        else
                            memorylocation[indextoinsert].begin1 = 0;

                        memorylocation[indextoinsert].end1 = memorylocation[indextoinsert].begin1
                                + memorylocation[indextoinsert].size1 - 1;

                        memorylocation[indextoinsert].spaceright = memorylocation[indextoinsert+1].spaceleft;
                    }

                    // inserting to best fit right of a process
                    // update space to left and right of that process
                    // set space to left and right of newly inserted process
                    // update space to left and right of process to right of new inserted process
                    // set where newly inserted process begins and ends
                    if(leftorright==1)
                    {
                        memorylocation.insert(memorylocation.begin() + indextoinsert+1, newprocess);

                        memorylocation[indextoinsert+1].spaceright = memorylocation[indextoinsert].spaceright
                                - memorylocation[indextoinsert+1].size1;

                        memorylocation[indextoinsert].spaceright = 0;

                        memorylocation[indextoinsert+1].spaceleft = 0;

                        if(!(indextoinsert+2 > memorylocationsize))
                            memorylocation[indextoinsert+2].spaceleft = memorylocation[indextoinsert+1].spaceright;

                        memorylocation[indextoinsert+1].begin1 = memorylocation[indextoinsert].end1+1;

                        memorylocation[indextoinsert+1].end1 = memorylocation[indextoinsert+1].begin1
                                + memorylocation[indextoinsert+1].size1 -1;
                    }
                    ready_enqueue(process);
                    if(cpu_status() == 0)
                        enter_cpu();
                    increase_next_PID();
                }
                else // if no room, cout no room
                {
                    cout << "No room available. Try again later." << endl;
                    cout << endl;
                }
            }
        }

        // Terminates process
        // If there's no process in CPU, print error message.
        // Find process in memory and remove it.
        // Update process to left of terminating process:
        //      add size of terminating process to spaceright.
        //      add spaceright of the terminating process.
        // Update process to left of terminating process:
        //      add size of terminating process to spaceleft.
        //      add spaceleft of the terminating process.
        void terminate_process()
        {
            if(cpu_status()==0)
            {
                cout << "No process in CPU. Termination denied." << endl;
                cout << endl;
            }
            else
            {
                PCB to_terminate = cpu[0];
                cpu.pop_back();
                int index=-1;
                int memorylocationsize = memorylocation.size();
                for(int i = 0; i < memorylocationsize; i++)
                {
                    if(memorylocation[i].pid == to_terminate.get_PID())
                        index = i;
                }

                if(!((index-1) < 0))
                {
                    memorylocation[index-1].spaceright = memorylocation[index-1].spaceright
                            + memorylocation[index].size1 + memorylocation[index].spaceright;
                }

                if(!(index+1 > memorylocationsize-1))
                {
                    memorylocation[index+1].spaceleft = memorylocation[index+1].spaceleft
                            + memorylocation[index].size1 + memorylocation[index].spaceleft;
                }

                memorylocation.erase(memorylocation.begin() + index);

                if(readyqueue.size()!=0)
                    enter_cpu();
                cout << "Process in CPU terminated." << endl;
                cout << endl;
            }
        }

        // Process wants to use printer
        // index parameter determines which printer to use.
        // Process is kicked out of CPU.
        // PCB, name of file to print, and size of file is stored in a PrinterData structure.
        // Put PrinterData in printer queue.
        // *Note: index-1 is used since printers are numbered starting at 1
        //      but are stored in a vector that starts indexing from 0.
        // If printer is idle, move first element in queue into printer.
        void access_printer(int index, string file_name, int file_size)
        {
            PCB process = cpu[0];
            cpu.pop_back();
            if(readyqueue.size()!=0)
                enter_cpu();
            PrinterData print;
            print.process = process;
            print.file_name = file_name;
            print.file_size = file_size;
            printers[index-1].enqueue_file(print);
            if(printers[index-1].printer_status() == 0)
                printers[index-1].use_printer();
        }

        // Process is done printing
        // index parameter determines which printer to use.
        // *Note: index-1 is used since printers are numbered starting at 1
        //      but are stored in a vector that starts indexing from 0.
        // Process leaves printer and enters ready queue.
        // Moves next process from ready queue into CPU.
        // Moves next process from printer queue into printer.
        // If this function is called while printer is idle and queue is empty, print error.
        void printer_completion(int index)
        {
            if(printers[index-1].printer_status() != 0)
            {
                PrinterData printed = printers[index-1].currently_printing();
                printers[index-1].done_with_printer();
                PCB process = printed.process;
                ready_enqueue(process);
                if(cpu_status() == 0)
                    enter_cpu();
                if((printers[index-1].printer_queue_size() != 0))
                    printers[index-1].use_printer();
                cout << "Printing task completed." << endl;
                cout << endl;
            }
            else
            {
                cout << "Error. No process is accessing printer and printer queue empty." << endl;
                cout << endl;
            }
        }

        // Process wants to use disk
        // index parameter determines which disk to use.
        // Process is kicked out of CPU.
        // PCB, name of file, size of file, and whether it's read or write
        //      is stored in a DiskData structure.
        // *Note: index-1 is used since disks are numbered starting at 1
        //      but are stored in a vector that starts indexing from 0.
        // Put DiskData in disk queue.
        // If disk is idle, move first element in queue into disk.
        void access_disk(int index, string file_name, int file_size, string readwrite)
        {
            PCB process = cpu[0];
            cpu.pop_back();
            if(readyqueue.size()!=0)
                enter_cpu();
            DiskData disk;
            disk.process = process;
            disk.file_name = file_name;
            disk.file_size = file_size;
            disk.read_or_write = readwrite;
            disks[index-1].enqueue_file(disk);
            if(disks[index-1].disk_status() == 0)
                disks[index-1].use_disk();
        }

        // Process is done with disk
        // index parameter determines which disk to use.
        // *Note: index-1 is used since disks are numbered starting at 1
        //      but are stored in a vector that starts indexing from 0.
        // Process leaves disk and enters ready queue.
        // Moves next process from ready queue into CPU.
        // Moves next process from disk queue into disk.
        // If this function is called while disk is idle and queue is empty, print error.
        void disk_completion(int index)
        {
            if(!disks[index-1].disk_status() == 0)
            {
                DiskData done = disks[index-1].current_disk_access();
                disks[index-1].done_with_disk();
                PCB process = done.process;
                ready_enqueue(process);
                if(cpu_status() == 0)
                    enter_cpu();
                if((disks[index-1].disk_queue_size() != 0))
                    disks[index-1].use_disk();
                cout << "Disk access completed." << endl;
                cout << endl;
            }
            else
            {
                cout << "Error. No process is accessing disk and disk queue empty." << endl;
                cout << endl;
            }
        }

        // Snapshot of ready queue
        // If CPU is empty, print idle. Otherwise print process in CPU.
        // If ready queue is not empty, print out all process in ready queue.
        // If ready queue is empty, print empty
        void snapshot_ready()
        {
            if(!cpu.empty())
            {
                cout << "CPU: " << cpu[0] << ", CPU Time: " << cpu[0].get_cpu_time()
                << ", Ready Time: " << cpu[0].get_ready_time() << endl;
            }
            else
            {
                cout << "CPU: Idle" << endl;
            }
            if(!readyqueue.empty())
            {
                int counter = 1;
                for(list<PCB>::iterator iter = readyqueue.begin(); iter != readyqueue.end(); ++iter)
                {
                    cout << counter << ": " << *iter << ", CPU Time: " << iter->get_cpu_time()
                    << ", Ready Time: " << iter->get_ready_time() <<endl;
                    counter++;
                }
            }
            else
            {
                cout << "Ready queue is empty." << endl;
            }
            cout << endl;
        }

        // Snapshot of printers
        // Prints current printing process and processes in queue for each printer.
        // If there are no printers, cout no printers.
        void snapshot_printers()
        {
            if(printers.size()!=0)
            {
                int printersize = printers.size();
                for(int i = 0; i < printersize; i++)
                    cout << "PRINTER " << i+1 << ": " << printers[i] << endl;
            }
            else
            {
                cout << "There are no printers." << endl;
                cout << endl;
            }
        }

        // Snapshot of disks
        // Prints current disk access process and processes in queue for each disk.
        // If there are no disks, cout no disks.
        void snapshot_disks()
        {
            if(disks.size()!=0)
            {
                int disksize = disks.size();
                for(int i = 0; i < disksize; i++)
                    cout << "DISK " << i+1 << ": " << disks[i] << endl;
            }
            else
            {
                cout << "There are no disks." << endl;
                cout << endl;
            }
        }

        // Snapshot of memory
        // Print locations in memory of all processes.
        void snapshot_memory()
        {
            if(memorylocation.size() != 0)
            {
                PCB temp_process;
                int memorylocationsize = memorylocation.size();
                for(int i = 0; i < memorylocationsize; i++)
                {
                    cout << memorylocation[i].begin1 << "-" << memorylocation[i].end1
                            << " : PID " << memorylocation[i].pid << ", CPU Time: "
                            << memorylocation[i].cpu_time << ", Ready Time: "
                            << memorylocation[i].ready_time;
                    cout << endl;
                }
                cout << endl;
            }
            else
            {
                cout << "There are currently no processes in memory." << endl;
                cout << endl;
            }
        }

        void increase_time_ready(int time)
        {
            for(list<PCB>::iterator iter = readyqueue.begin(); iter != readyqueue.end(); ++iter)
            {
                iter->increase_ready_time(time);
                int memorylocationsize = memorylocation.size();
                for(int i = 0; i < memorylocationsize; i++)
                {
                    if(memorylocation[i].pid == iter->get_PID())
                        memorylocation[i].ready_time += time;
                }
            }
        }

        void increase_time_cpu(int time)
        {
            cpu[0].increase_cpu_time(time);
            int memorylocationsize = memorylocation.size();
            for(int i = 0; i < memorylocationsize; i++)
            {
                if(memorylocation[i].pid == cpu[0].get_PID())
                    memorylocation[i].cpu_time += time;
            }
        }
    private:
        int memory;
        int numberOfPrinters;
        int numberOfDisks;
        int pidCounter = 0;
        vector<MemData> memorylocation;
        vector<PCB> cpu;
        list<PCB> readyqueue;
        vector<Printer> printers;
        vector<Disk> disks;
};
#endif
