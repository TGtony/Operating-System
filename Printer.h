/**********************
    Tony Gong
    CSCI 340
    OS Project
**********************/

#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "PCB.h"

using namespace std;

struct PrinterData
{
    PCB process;
    string file_name;
    int file_size;

    friend ostream &operator<< (ostream &out, const PrinterData &print)
    {
        cout << print.process << " -> File: " << print.file_name
                << ", File Size: " << print.file_size << ", CPU time: "
                << print.process.get_cpu_time() << ", Ready time: "
                << print.process.get_ready_time() << endl;
        return out;
    }
}; // Structure used to hold the PCB and related data for printers

class Printer
{
    public:
        // 1st process in queue uses printer
        void use_printer()
        {
            current_file.push_back(printer_queue.front());
            printer_queue.pop_front();
        }

        // Process is done printing
        void done_with_printer()
        {
            current_file.pop_back();
        }

        // Put process in printer queue
        void enqueue_file(PrinterData file)
        {
            printer_queue.push_back(file);
        }

        // Return current printing process
        PrinterData currently_printing()
        {
            return current_file[0];
        }

        // Checks if printer is idle
        int printer_status()
        {
            return current_file.size();
        }

        // Checks if printer queue is empty
        int printer_queue_size()
        {
            return printer_queue.size();
        }

        // Overload << to cout printer and queues
        // If there's no process in printer, print idle.
        // If there is a process, print the process
        // If the queue is not empty, print every element in queue
        // If the queue is empty, print queue is empty
        friend ostream &operator<< (ostream &out, const Printer &rhs)
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
            if(rhs.printer_queue.size() != 0)
            {
                for(list<PrinterData>::const_iterator iter = rhs.printer_queue.begin();
                        iter != rhs.printer_queue.end(); ++iter)
                {
                    cout << counter << ": " << *iter;
                    counter++;
                }
            }
            else
            {
                cout << "Printer queue is empty." << endl;
            }
            return out;
        }

    private:
        vector<PrinterData> current_file;
        list<PrinterData> printer_queue;
};
#endif
