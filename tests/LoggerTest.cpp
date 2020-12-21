#include "Logger.h"
#include "Strings.h"
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

enum State
{
    SUCCESS,
    FAILURE
};

class LoggerTest
{
public:
    static State state;
    static std::ifstream testFile;
    static std::string testString;
    static std::string requiredString;

    inline static void initTest()
    {
        Logger::init(Logger::DEBUG_MODE::ON, Logger::BUFFERED_LOGGING::ON, 10, "testLog.txt");
        if (testFile.peek() == std::ifstream::traits_type::eof())
            return;
        printf("initTest: File is not empty");
        state = FAILURE;
    }
    inline static void openFileTest()
    {
        testFile.open(Logger::userFileName, std::ios::out | std::ios::app);
        if (testFile.is_open())
        {
            testFile.close();
            return;
        }
        state = FAILURE;
        std::cout << "Unable to open file from path\n";
    }
    inline static void resetTimeTest()
    {
        time_t oldTime = Logger::currentTime;
        usleep(1000000);
        Logger::resetTime();
        if (Logger::currentTime > oldTime)
            return;

        printf("resetTimeTest: Time before reset (%f) is not older than new time(%f).\n", (double)Logger::currentTime, (double)oldTime);
        state = FAILURE;
    }
    inline static void writeMessageTest()
    {
        Logger::writeMessage("Info test", "[INFO]");
        requiredString = "[INFO]: Info test |" + std::string(ctime(&Logger::currentTime));
        testFile.open(Logger::userFileName);

        getline(testFile, testString);
        getline(testFile, testString);
        testString.append("\n");
        testFile.close();
        remove(Logger::userFileName);
        if (testString.compare(requiredString) == 0)
            return;

        state = FAILURE;
        std::cout << "LoggeTest::info: Messages doesn't match: " << testString << "\n"
                  << "VS. " << requiredString;
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
    LoggerTest::writeMessageTest();
    return LoggerTest::state;
}