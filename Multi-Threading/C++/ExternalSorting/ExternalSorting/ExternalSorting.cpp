// ExternalSorting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// System defined includes
#include <condition_variable>
#include <fstream>
#include <filesystem> 
#include <iostream>
#include <mutex>
#include <queue>
#include <random>  
#include <string>
#include <sstream>

// User defined includes
#include "Constants.h"
#include "Util.h"

using namespace std;
namespace fs = std::filesystem;

std::queue<std::vector<int>>    g_queue;
std::condition_variable         g_cond;
std::mutex                      g_mutex;
std::mutex                      g_FileLock; /// for unique file names
std::mutex                      g_queueLock; /// for add remove lock
bool                            g_IsProcessingDone = false;
fs::path                        g_passNo;

fs::path uniqueName(const std::string& name) {
    std::lock_guard <std::mutex> lock(g_FileLock);
    fs::path possibleName{ g_passNo /name };
    auto stem = possibleName.stem().string();
    auto ext = possibleName.extension().string();
    for (int i = 1; fs::exists(possibleName); ++i) {
        std::ostringstream fn;
        fn << stem << i << ext;
        possibleName.replace_filename(fn.str());
    }
    return possibleName;
}

// read the file in single thread only, sequentially, as the I/O operation will cause system to slow down
// if done in multi-threaded way
void Producer()
{
    std::lock_guard<std::mutex> guard(g_mutex);
    cout << "Producer started\n";
    std::ifstream inFile(FILENAME);
    if (inFile.fail())
    {
        cout << "Cannot open file " << FILENAME << NEWLINE;
        exit(1);
    }

    string line;
    int count = 0;
    std::vector <int> lVec;
    while (std::getline (inFile, line ))
    {
        if (count == 1024)
        {
            g_queue.emplace(std::move(lVec));
            count = 0;
            g_cond.notify_one();
        }
        
        if (line.size() > 0)
        {
            if (count < BUFFER_SIZE)
            {
                lVec.push_back(stoi(line));
                count++;
            }
        }
    }
    
    // if count has some value but last chunk does not contain the 1024 digits
    if (count > 0)
    {
        g_queue.emplace(std::move(lVec));
        count = 0;
    }

    inFile.close();
    cout << "Producer finished\n";
    g_IsProcessingDone = true;
    g_cond.notify_all();
}

void Consumer(int threadId)
{
    while (true)
    {
        std::vector<int> element;
        {
            std::unique_lock<std::mutex> mLock(g_queueLock);
            g_cond.wait(mLock, [] { return (g_queue.size() > 0 || g_IsProcessingDone); });
            if (g_queue.size() == 0 && g_IsProcessingDone)
            {
                cout << "thread " << threadId << " exits." << NEWLINE;
                break;
            }
            cout << "thread " << threadId << " remaining chunks are " << g_queue.size() << NEWLINE;
            element = g_queue.front();
            g_queue.pop();
        }
        
        std::ofstream outFile(uniqueName(TEMPFILE));
        sort(element.begin(), element.end());
        for(const auto &line : element )
        {
            outFile << line << NEWLINE;
        }

        outFile.close();
        if (g_queue.size() == 0 && g_IsProcessingDone)
        {
            cout << "thread " << threadId << " exits." << NEWLINE;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void RunPass(string file1 = "", string file2 = "")
{
    vector <int> lVec;
    if (!file1.empty())
    {
        std::ifstream firstFile(file1);
        string line;
        while (std::getline(firstFile, line))
        {
            if (!line.empty())
                lVec.push_back(stoi(line));
        }
        firstFile.close();
        fs::remove(file1);
    }

    if (!file2.empty())
    {
        std::ifstream secondFile(file2);
        string line;
        while (std::getline(secondFile, line))
        {
            if (!line.empty())
                lVec.push_back(stoi(line));
        }
        secondFile.close();
        fs::remove(file2);
    }
    std::ofstream outFile(uniqueName(TEMPFILE));
    std::sort(lVec.begin(), lVec.end());
    for (const auto& data : lVec)
    {
        outFile << data << NEWLINE;
    }
    outFile.close();
}

int main()
{
    // CUtil::GeneraterRandomNumber();
    
    // CUtil::SortInputFile();

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    CUtil::CleanUp();

    g_passNo = fs::current_path().string() + '\\' + DIR_PASS + std::to_string(0);
    if (!fs::exists(g_passNo))
        fs::create_directories(g_passNo);

    std::thread tProducer(Producer);

    std::thread tConsumer[10];

    for (size_t i = 0; i < 10; i++)
    {
        tConsumer[i] = std::thread (Consumer, i);
    }

    tProducer.join();

    cout << "consumer threads created\n";
    for (size_t i = 0; i < 10; i++)
    {
        tConsumer[i].join();
    }

    // we are done with pass-0 so now we need to apply sorting algorithm iteratively
    int passNo = 1;
    while (true)
    {
        std::vector<string> files;
        for (const auto& filePath : fs::directory_iterator(g_passNo))
        {            
            files.push_back(filePath.path().string()); // this we will remove as assuming we have very less memory
        }

        // run till we are remaining with one file
        if (files.size() == 1)
        {
            // here rename the file to output something. and do the clean up of other folders
            fs::copy(files[0], fs::current_path()/ OUTPUT_FILE, fs::copy_options::overwrite_existing);
            CUtil::CleanUp();
            break;
        }

        // create a directory for new pass
        g_passNo = fs::current_path().string() + '\\' + DIR_PASS + std::to_string(passNo);
        if (!fs::exists(g_passNo))
            fs::create_directories(g_passNo);
        
        size_t i = 0;
        // std::thread lThread[100]; -- not giving much benefit of threading
        // int count = 0;
        while (i < files.size())
        {
            auto file1 = files[i++];
            auto file2 = (i + 1 <= files.size()) ? files[i++] : "";
            RunPass( file1, file2);
            // lThread[count] = std::thread(RunPass, file1, file2); //  -- not giving much benefit of threading
            // lThread[count++].join(); // -- not giving much benefit of threading
        }
        passNo++;
    }
   
    cout << "processing finished\n";
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cout << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << NEWLINE;
}
