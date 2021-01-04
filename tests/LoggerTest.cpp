#include "Logger.h"
#include "Strings.h"
#include "Commons.h"
#include <string>
#include <unistd.h>

class LoggerTest
{
public:
    static State state;
    static std::ifstream testFile;
    static std::string testString;
    static std::string requiredString;

    inline static void initTest()
    {
        Logger::init(Logger::DEBUG_MODE::ON, Logger::BUFFERED_LOGGING::OFF, 10, "testLog.txt");
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
        usleep(1000000);
        Logger::resetTime();
        if (Logger::currentTime > oldTime)
            return;

        printf("resetTimeTest: Time before reset (%f) is not older than new time(%f).\n", (double)Logger::currentTime, (double)oldTime);
        state = State::FAILURE;
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