#include "Logger.h"
#include "Strings.h"
#include "Commons.h"

#include <cstring>
#include <string>

class LoggerTest
{
public:
    static State state;
    static std::ifstream testFile;
    static std::string testString;
    static std::string requiredString;

    inline static void initTest()
    {
        Logger::init(Logger::WRITE_TO_FILE, "testLog.txt");
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
    LoggerTest::infoMessageTest();
    return !(LoggerTest::state == State::SUCCESS);
}