//
//  main.cpp
//  Lab05
//
//  Created by Alex Solorio on 1/23/17.
//  Copyright © 2017 Alex Solorio. All rights reserved.
//
//  Practicing reading, writing, and navigating binary files using the example of storing student records
//

#include <iostream>
#include <stdio.h>
#include <assert.h>

struct Student {
    int studentId;
    int assignmentGrades[4];
};

int main()
{
    FILE *studentRecords;
    studentRecords = fopen("student_records.bin", "wb+");
    
    //Writing all student records
    Student currentStudent;
    for(int i = 0; i < 5; i++)
    {
        currentStudent.studentId = i;
        std::cout << "What are the 4 grades for student #" << i << "?\n";
        for(int j = 0; j < 4; j++)
        {
            std::cin >> currentStudent.assignmentGrades[j];
        }
        long checkWrite = fwrite(&currentStudent, sizeof(Student), 1, studentRecords);
        assert (checkWrite == 1);
    }
    
    //Reading all student records in reverse
    for(int i = 4; i >= 0; i--)
    {
        long checkSeek = fseek(studentRecords, sizeof(Student) * i, SEEK_SET);
        assert (checkSeek == 0);
        long checkRead = fread(&currentStudent, sizeof(Student), 1, studentRecords);
        assert (checkRead == 1);
        
        //print ID and grades
        std::cout << "Student ID: " << currentStudent.studentId << "\n";
        std::cout << "Student Grades: \n";
        for(int j = 0; j < 4; j++)
        {
            std::cout << currentStudent.assignmentGrades[j] << "\n";
        }
        
        checkSeek = fseek(studentRecords, sizeof(Student) * i, SEEK_SET);
        assert (checkSeek == 0);
        std::cout << "Please enter 4 new grades for student #" << i << "\n";
        for(int j = 0; j < 4; j++)
        {
            std::cin >> currentStudent.assignmentGrades[j];
        }
        long checkWrite = fwrite(&currentStudent, sizeof(Student), 1, studentRecords);
        assert (checkWrite == 1);
    }
    
    //Reading all student records in reverse
    for(int i = 4; i >= 0; i--)
    {
        long checkSeek = fseek(studentRecords, sizeof(Student) * i, SEEK_SET);
        assert (checkSeek == 0);
        long checkRead = fread(&currentStudent, sizeof(Student), 1, studentRecords);
        assert (checkRead == 1);
        
        //print ID and grades
        std::cout << "Student ID: " << currentStudent.studentId << "\n";
        std::cout << "Student Grades: \n";
        for(int j = 0; j < 4; j++)
        {
            std::cout << currentStudent.assignmentGrades[j] << "\n";
        }
        
        checkSeek = fseek(studentRecords, sizeof(Student) * i, SEEK_SET);
        assert (checkSeek == 0);
        std::cout << "Please enter 4 new grades for student #" << i << "\n";
        for(int j = 0; j < 4; j++)
        {
            std::cin >> currentStudent.assignmentGrades[j];
        }
        long checkWrite = fwrite(&currentStudent, sizeof(Student), 1, studentRecords);
        assert (checkWrite == 1);
    }
    
    fclose(studentRecords);
}
