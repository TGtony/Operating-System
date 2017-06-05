/**********************
    Tony Gong
    CSCI 340
    OS Project
**********************/

#include <iostream>
#include <string>
#include <sstream>
#include "PCB.h"
#include "Printer.h"
#include "Disk.h"
#include "OS.h"

using namespace std;

int main()
{
    // Generate System
    int numPrint;
    int numDisk;
    int mem;
    cout << "Number of Printers: ";
    cin >> numPrint;
    cout << "Number of Disks: ";
    cin >> numDisk;
    cout << "Memory: ";
    cin >> mem;
    cout << endl;
    OS system(mem, numPrint, numDisk);
    system.generate_printers();
    system.generate_disks();

    while(true)
    {
        string command;
        cout << "Enter command: ";
        cin >> command;
        cout << endl;
        if(command.length() == 1)
        {
            if(command == "A") // Create process
            {
                int time;
                cout << "How much time has passed?: ";
                cin >> time;
                system.increase_time_ready(time);
                if(system.cpu_status() != 0)
                    system.increase_time_cpu(time);
                int sizeofprocess;
                cout << "How much memory for process?: ";
                cin >> sizeofprocess;
                cout << endl;
                int pid = system.pidAssign();
                PCB process = PCB(sizeofprocess, pid);
                system.allocate_memory(process);
            }
            else if(command == "t") // Terminate process
            {
                int time;
                cout << "How much time has passed?: ";
                cin >> time;
                system.increase_time_ready(time);
                if(system.cpu_status() != 0)
                    system.increase_time_cpu(time);
                system.terminate_process();
            }
            else if(command == "S") // Snapshot
            {
                int time;
                cout << "How much time has passed?: ";
                cin >> time;

                cout << "What kind of snapshot? : ";
                string snapshot;
                cin >> snapshot;
                cout << endl;

                if(snapshot == "r") // Snapshot of ready
                {
                    system.increase_time_ready(time);
                    if(system.cpu_status() != 0)
                        system.increase_time_cpu(time);

                    system.snapshot_ready();
                }
                else if(snapshot == "p") // Snapshot of printer
                {
                    system.increase_time_ready(time);
                    if(system.cpu_status() != 0)
                        system.increase_time_cpu(time);

                    system.snapshot_printers();
                }
                else if(snapshot == "d") // Snapshot of disk
                {
                    system.increase_time_ready(time);
                    if(system.cpu_status() != 0)
                        system.increase_time_cpu(time);

                    system.snapshot_disks();
                }
                else if(snapshot == "m") // Snapshot of memory
                {
                    system.increase_time_ready(time);
                    if(system.cpu_status() != 0)
                        system.increase_time_cpu(time);

                    system.snapshot_memory();
                }
                else
                {
                    cout << "Invalid snapshot command." << endl;
                    cout << endl;
                }
            }
            else
            {
                cout << "Invalid command." << endl;
                cout << endl;
            }
        }

        else if(command.length() == 2) // 1st char -> command type, 2nd char -> which printer/disk
        {
            string commandS = command.substr(1,1);
            stringstream commandstream(commandS);
            if(command[0] == 'p') // request printer
            {
                int printer_number;
                commandstream >> printer_number;
                if(printer_number > numPrint)
                {
                    cout << "Error. There is/are only " << numPrint << " printer(s)." << endl;
                    cout << "Printer number " << printer_number << " does not exist." << endl;
                    cout << endl;
                }
                else
                {
                    int time;
                    cout << "How much time has passed?: ";
                    cin >> time;
                    system.increase_time_ready(time);
                    if(system.cpu_status() != 0)
                        system.increase_time_cpu(time);

                    cout << "Request for printer " << printer_number << endl;
                    if(!system.cpu_status() == 0)
                    {
                        string file_name;
                        cout << "File name: ";
                        cin >> file_name;
                        int file_size;
                        cout << "File size: ";
                        cin >> file_size;
                        cout << endl;
                        system.access_printer(printer_number, file_name, file_size);
                    }
                    else
                    {
                        cout << "Error. There's no process in CPU." << endl;
                        cout << endl;
                    }
                }
            }
            else if(command[0] == 'P') // printing job done
            {
                int printer_number;
                commandstream >> printer_number;
                if(printer_number > numPrint)
                {
                    cout << "Error. There is/are only " << numPrint << " printer(s)." << endl;
                    cout << "Printer number " << printer_number << " does not exist." << endl;
                    cout << endl;
                }
                else
                {
                    int time;
                    cout << "How much time has passed?: ";
                    cin >> time;
                    system.increase_time_ready(time);
                    if(system.cpu_status() != 0)
                        system.increase_time_cpu(time);

                    system.printer_completion(printer_number);
                }
            }
            else if(command[0] == 'd') // request disk
            {
                int disk_number;
                commandstream >> disk_number;
                if(disk_number > numDisk)
                {
                    cout << "Error. There is/are only " << numDisk << " disk(s)." << endl;
                    cout << "Disk number " << disk_number << " does not exist." << endl;
                    cout << endl;
                }
                else
                {
                    int time;
                    cout << "How much time has passed?: ";
                    cin >> time;
                    system.increase_time_ready(time);
                    if(system.cpu_status() != 0)
                        system.increase_time_cpu(time);

                    cout << "Request for disk " << disk_number << endl;
                    if(!system.cpu_status() == 0)
                    {
                        string file_name;
                        cout << "File name: ";
                        cin >> file_name;
                        int file_size;
                        cout << "File size: ";
                        cin >> file_size;
                        string readwrite;
                        cout << "Read or Write (r/w)?: ";
                        cin >> readwrite;
                        cout << endl;

                        if(readwrite == "r")
                            system.access_disk(disk_number, file_name, file_size, "Read");

                        else if(readwrite == "w")
                            system.access_disk(disk_number, file_name, file_size, "Write");

                        else
                        {
                            cout << "Error. User entered something besides 'r' or 'w'." << endl;
                            cout << "Request has been denied. Try again." << endl;
                            cout << endl;
                        }
                    }
                    else
                    {
                        cout << "Error. There's no process in CPU." << endl;
                        cout << endl;
                    }
                }
            }
            else if(command[0] == 'D') // disk access done
            {
                int disk_number;
                commandstream >> disk_number;
                if(disk_number > numDisk)
                {
                    cout << "Error. There is/are only " << numDisk << " disk(s)." << endl;
                    cout << "Disk number " << disk_number << " does not exist." << endl;
                    cout << endl;
                }
                else
                {
                    int time;
                    cout << "How much time has passed?: ";
                    cin >> time;
                    system.increase_time_ready(time);
                    if(system.cpu_status() != 0)
                        system.increase_time_cpu(time);

                    system.disk_completion(disk_number);
                }
            }
            else
            {
                cout << "Invalid command." << endl;
                cout << endl;
            }
        }
        else
        {
            cout << "Invalid command." << endl;
            cout << endl;
        }
    }
    return 0;
}
