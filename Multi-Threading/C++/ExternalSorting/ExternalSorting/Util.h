#pragma once

#include <iostream>
#include <fstream>
#include <filesystem> 
#include <string>
#include "Constants.h"
#include <sstream>

using namespace std;
namespace fs = std::filesystem;

class CUtil
{
public:
    static void CleanUp()
    {
        // remove the folders starts with DIR_PASS 
        for (const auto& entry : fs::directory_iterator(fs::current_path()))
        {
            if (entry.is_directory())
            {
                auto path = entry.path().stem().string();
                if (path.find(DIR_PASS) != std::string::npos)
                {
                    fs::remove_all(entry.path());
                }
            }
        }
    }

    static void GeneraterRandomNumber()
    {
        std::ofstream outFile;
        outFile.open("Records.txt", ios::out);
        int count = 0;
        unsigned long j = 0;
        srand(NULL);
        while (count++ < 100000)
        {
            outFile << rand() << "\n";
        }
        outFile.close();
    }

    // this function is just for comparison of our output file with input file.
    static void SortInputFile()
    {
        ifstream inFile("Records.txt", ios::in);
        ofstream outFile("RecordsSort.txt", ios::out);

        string line;
        vector <int> lVec;
        while (std::getline(inFile, line))
        {
            lVec.push_back(stoi(line));
        }

        std::sort(lVec.begin(), lVec.end());

        for (const auto& data : lVec)
        {
            outFile << data << NEWLINE;
        }

        inFile.close();
        outFile.close();
    }

    uintmax_t getFileSize(string fileName)
    {
        uintmax_t fileSize = 0;
        try
        {
            fileSize = fs::file_size(fileName);
        }
        catch (fs::filesystem_error& fc)
        {
            cout << fc.what();
        }
        return fileSize;
    }
};