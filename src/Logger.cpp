#include "Logger.h"

#include <cstring>
#include <algorithm>
#include <chrono>

std::vector<Logger::Message> Logger::messageStack;
time_t Logger::currentTime;
std::ofstream Logger::logFile;
Logger::BUFFERED_LOGGING Logger::bufferedLogging;
Logger::DEBUG_MODE Logger::debugMode;
size_t Logger::userStackSize;


void Logger::init(DEBUG_MODE dm, BUFFERED_LOGGING bl, const size_t userSize)
{
    debugMode = dm;
    bufferedLogging = bl;
    userStackSize = userSize;
    resetTime();
    logFile.open(LoggerTags::PATH, std::ios::out | std::ios::trunc);
    logFile.close();
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
        printSystemMessage("Logger::flush: Trying to flush empty container."); //TODO DEBUG
        return;
    }

    logFile.open(LoggerTags::PATH, std::ios::out | std::ios::app);
    if (!logFile.is_open())
    {
        printSystemMessage("Logger::flush: ERROR: Unable to open file!");
        return;
    }
    std::for_each(begin(messageStack), end(messageStack), [](const Message &msg) { write(msg.message, msg.msgType); });
    logFile.close();
    messageStack.clear();
    messageStack.reserve(userStackSize);
}

void Logger::stackMessage(const char *message, const char *typeHeader)
{
    if (messageStack.capacity() == 0 && messageStack.size() != userStackSize)
    {
        messageStack.reserve(userStackSize);
    }

    messageStack.emplace_back(message, typeHeader, currentTime);

    if (messageStack.size() >= userStackSize || strcmp(typeHeader, LoggerTags::ERROR) == 0)
    {
        flush();
    }
}

void Logger::resetTime()
{
    currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void Logger::writeMessage(const char *message, const char *typeHeader)
{
    logFile.open(LoggerTags::PATH, std::ios::out | std::ios::app);

    if (!logFile.is_open())
    {
        printSystemMessage("Logger::writeMessage: ERROR: Unable to open file!");
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