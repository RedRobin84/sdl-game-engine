#include "Logger.h"
#include "Commons.h"

#include <cstdio>
#include <time.h>
#include <cstring>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std::literals::chrono_literals;

class LoggerTest
{
public:
inline static const std::string TEST_FILE_NAME{"testLog.txt"};
    static State state;
    static std::ifstream testFile;
    static std::string testString;
    static std::string requiredString;

    inline static void initTest()
    {
        Logger::init(Logger::WRITE_TO_FILE, TEST_FILE_NAME.c_str());
        if (testFile.peek() == std::ifstream::traits_type::eof())
            return;
        printf("initTest: File is not empty\n");
        state = State::FAILURE;
    }
    inline static void openFileTest()
    {
        testFile.open(Logger::userFileName, std::ios::out | std::ios::app);
        if (testFile.is_open())
        {
            testFile.close();
            return;
        }
        state = State::FAILURE;
        printf("openFileTest: Unable to open file from path\n");
    }
    inline static void resetTimeTest()
    {
        time_t oldTime = Logger::currentTime;
        std::this_thread::sleep_for(1000ms);
        Logger::resetTime();
        if (Logger::currentTime > oldTime)
            return;

        printf("resetTimeTest: Time before reset (%f) is not older than new time(%f).\n", (double)Logger::currentTime, (double)oldTime);
        state = State::FAILURE;
        printf("openFileTest: Unable to open file from path");
    }
    inline static void infoMessageTest()
    {
        int i = 3;
        const char * string = "test string";
        Logger::info("Test: number = %d, string = %s", i, string);
        requiredString = "[" + std::string(std::strtok(ctime(&Logger::currentTime), "\n")) + "]" + "[INFO]: Test: number = 3, string = test string";
        testFile.open(Logger::userFileName);
        getline(testFile, testString);
        getline(testFile, testString);
        testFile.close();
        remove(Logger::userFileName);
        if (testString.compare(requiredString) == 0)
            return;

        state = State::FAILURE;
        printf("LoggeTest::info: Messages doesn't match: %s\nVS. %s ", testString.data(), requiredString.data());
    }
};

State LoggerTest::state = State::SUCCESS;
std::ifstream LoggerTest::testFile;
std::string LoggerTest::testString;
std::string LoggerTest::requiredString;

int main()
{
    LoggerTest::initTest();
    LoggerTest::openFileTest();
    LoggerTest::resetTimeTest();
    LoggerTest::infoMessageTest();
    return !(LoggerTest::state == State::SUCCESS);
}