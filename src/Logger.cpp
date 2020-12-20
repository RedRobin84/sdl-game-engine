#include "Logger.h"

#include <cstring>
#include <algorithm>
#include <chrono>

std::vector<Logger::Message> Logger::messageStack;
time_t Logger::currentTime;
std::ofstream Logger::logFile;
const char * Logger::userFileName = DEFAULT_FILE_NAME;
Logger::BUFFERED_LOGGING Logger::bufferedLogging = BUFFERED_LOGGING::OFF;
Logger::DEBUG_MODE Logger::debugMode = DEBUG_MODE::OFF;
size_t Logger::userStackSize = DEFAULT_STACK_SIZE;

void Logger::init(DEBUG_MODE dm, BUFFERED_LOGGING bl, const size_t userSize, const char * userFN)
{
    debugMode = dm;
    bufferedLogging = bl;
    userStackSize = userSize;
    userFileName = userFN;
    resetTime();
    fileCheck();
    printSystemMessage(LoggerTags::INIT_MESSAGE);
    printSystemMessage(debugMode == DEBUG_MODE::ON ? LoggerTags::DEBUG_MODE_ON : LoggerTags::DEBUG_MODE_OFF);
    printSystemMessage(bufferedLogging == BUFFERED_LOGGING::ON ? LoggerTags::BUFFERED_LOGGING_ON : LoggerTags::BUFFERED_LOGGING_OFF);
}

void Logger::enableBufferedLogging()
{
    if (bufferedLogging == BUFFERED_LOGGING::ON)
    {
        printSystemMessage("Buffered logging already on.");
        return;
    }

    bufferedLogging = BUFFERED_LOGGING::ON;
    messageStack.reserve(userStackSize);
}
void Logger::disableBufferedLogging()
{
    if (bufferedLogging == BUFFERED_LOGGING::OFF)
    {
        printSystemMessage("Buffered logging already off.");
        return;
    }

    bufferedLogging = BUFFERED_LOGGING::OFF;
    flush();
    messageStack.clear();
}

void Logger::info(const char *message)
{
    handleMessage(LoggerTags::GREEN, LoggerTags::INFO, message);
}

void Logger::warn(const char *message)
{
    handleMessage(LoggerTags::YELLOW, LoggerTags::WARN, message);
}

void Logger::error(const char *message)
{
    handleMessage(LoggerTags::RED, LoggerTags::ERROR, message);
}

void Logger::debug(const char *message)
{
    if (bufferedLogging == BUFFERED_LOGGING::OFF) return;
    handleMessage(LoggerTags::WHITE, LoggerTags::DEBUG, message);
}

void Logger::handleMessage(const char *color, const char *typeHeader, const char *message)
{
    resetTime();
    printMessage(color, typeHeader, message);
    bufferedLogging == BUFFERED_LOGGING::ON ? 
        stackMessage(message, typeHeader) : 
        writeMessage(message, typeHeader);
}

void Logger::printSystemMessage(const char *message)
{
    printMessage(LoggerTags::CYAN, LoggerTags::LOGGER, message);
}

void Logger::printMessage(const char *color, const char *typeHeader, const char *message)
{
    printf("%s%s%s%s%s %s -%s", LoggerTags::CLR_ESC, color, typeHeader, LoggerTags::CLR_ESC, LoggerTags::RESET, message, ctime(&currentTime));
}

void Logger::flush()
{
    if (messageStack.empty())
    {
        printSystemMessage("flush: Trying to flush empty container."); //TODO DEBUG
        return;
    }

    logFile.open((userFileName ? userFileName : DEFAULT_FILE_NAME), std::ios::out | std::ios::app);
    if (logFile.bad())
    {
        printSystemMessage("flush: Unable to write to file!");
        return;
    }
    std::for_each(begin(messageStack), end(messageStack), [](const Message &msg) { write(msg.message, msg.msgType); });
    logFile.close();
    messageStack.clear();
    messageStack.reserve(userStackSize);
}

void Logger::stackMessage(const char *message, const char *typeHeader)
{
    if (messageStack.capacity() == 0)
    {
        if (debugMode == DEBUG_MODE::ON) printSystemMessage("stackMessage: Not enough capacity. Extending...");
        messageStack.reserve(userStackSize);
    }

    messageStack.emplace_back(message, typeHeader, currentTime);

    if (messageStack.size() >= userStackSize || strcmp(typeHeader, LoggerTags::ERROR) == 0)
    {
        if (debugMode == DEBUG_MODE::ON) printSystemMessage("stackMessage: Container full or message of type [ERROR] = flush()");
        flush();
    }
}

void Logger::resetTime()
{
    currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void Logger::writeMessage(const char *message, const char *typeHeader)
{
    logFile.open(userFileName, std::ios::out | std::ios::app);

    if (logFile.bad())
    {
        printSystemMessage("writeMessage: Unable to write to file!");
        return;
    }
    write(message, typeHeader);
    logFile.close();
}

void Logger::write(const char *message, const char *typeHeader, time_t time)
{
    logFile  << typeHeader << ": " << message << " - " << (time ? ctime(&time) : ctime(&currentTime));
}

void Logger::finalCleanup()
{
    if (bufferedLogging == BUFFERED_LOGGING::ON && !messageStack.empty())
    {
        flush();
    }
    printSystemMessage(LoggerTags::EXIT_MESSAGE);
}

void Logger::fileCheck()
{
    logFile.open(userFileName, std::ios::out | std::ios::app);
    if (logFile.bad()) logFile.open(userFileName, std::ios::out | std::ios::trunc);
    logFile.close();
}