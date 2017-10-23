//
//  main.cpp
//  Lab09
//
//  Created by Alex Solorio on 2/8/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  This project shows how much of an impact cache optimization can have on alrogithms (in this case, simple array initialization)
//

#include <iostream>

using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::duration;
using std::cout;


int main()
{
    {
        const int size = 4000;
        int *data = new int[size*size];
        
        cout << "2D bad cache optimization:\n";
        {
            time_point<system_clock> start, end;
            start = std::chrono::system_clock::now();
            
            for (int t = 0; t < 20; t++)
            {
                for (int x = 0; x < size; x++)
                {
                    for (int y = 0; y < size; y++)
                    {
                        data[y*size + x] += x;
                    }
                }
            }
            
            end = std::chrono::system_clock::now();
            duration<double> elapsed_seconds = end-start;
            cout << "Elapsed time: " << elapsed_seconds.count() << "s\n\n";
        }
        cout << "\n2D good cache optimization:\n";
        {
            time_point<system_clock> start, end;
            start = std::chrono::system_clock::now();
            
            for (int t = 0; t < 20; t++)
            {
                for (int y = 0; y < size; y++)
                {
                    for (int x = 0; x < size; x++)
                    {
                        data[y*size + x] += x;
                    }
                }
            }
            
            end = std::chrono::system_clock::now();
            duration<double> elapsed_seconds = end-start;
            cout << "Elapsed time: " << elapsed_seconds.count() << "s\n\n";
        }
    }
    
    {
        const int size = 400;
        int *data = new int[size*size*size];
        
        cout << "\n3D bad cache optimization:\n";
        {
            time_point<system_clock> start, end;
            start = std::chrono::system_clock::now();
            
            for (int t = 0; t < 10; t++)
            {
                for (int x = 0; x < size; x++)
                {
                    for (int y = 0; y < size; y++)
                    {
                        for (int z = 0; z < size; z++)
                        {
                            data[(z*size*size) + (y*size) + x] += x;
                        }
                    }
                }
            }
            
            end = std::chrono::system_clock::now();
            duration<double> elapsed_seconds = end-start;
            cout << "Elapsed time: " << elapsed_seconds.count() << "s\n\n";
        }
        cout << "\n3D somewhat better cache optimization:\n";
        {
            time_point<system_clock> start, end;
            start = std::chrono::system_clock::now();
            
            for (int t = 0; t < 10; t++)
            {
                for (int y = 0; y < size; y++)
                {
                    for (int x = 0; x < size; x++)
                    {
                        for (int z = 0; z < size; z++)
                        {
                            data[(z*size*size) + (y*size) + x] += x;
                        }
                    }
                }
            }
            
            end = std::chrono::system_clock::now();
            duration<double> elapsed_seconds = end-start;
            cout << "Elapsed time: " << elapsed_seconds.count() << "s\n\n";
        }
        cout << "\n3D good cache optimization:\n";
        {
            time_point<system_clock> start, end;
            start = std::chrono::system_clock::now();
            
            for (int t = 0; t < 10; t++)
            {
                for (int z = 0; z < size; z++)
                {
                    for (int y = 0; y < size; y++)
                    {
                        for (int x = 0; x < size; x++)
                        {
                            data[(z*size*size) + (y*size) + x] += x;
                        }
                    }
                }
            }
            
            end = std::chrono::system_clock::now();
            duration<double> elapsed_seconds = end-start;
            cout << "Elapsed time: " << elapsed_seconds.count() << "s\n\n";
        }
        
    }

}
