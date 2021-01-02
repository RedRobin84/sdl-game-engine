#include "Logger.h"

#include <cstring>
#include <algorithm>
#include <chrono>

std::vector<std::unique_ptr<char[]>> Logger::messageStack;
time_t Logger::currentTime;
FILE *Logger::logFile;
const char *Logger::userFileName = DEFAULT_FILE_NAME;
Logger::BUFFERED_LOGGING Logger::bufferedLogging = BUFFERED_LOGGING::OFF;
Logger::DEBUG_MODE Logger::debugMode = DEBUG_MODE::OFF;
size_t Logger::userStackSize = DEFAULT_STACK_SIZE;
uint16_t Logger::userBufferSize = DEFAULT_BUFFER_SIZE;

void Logger::init(DEBUG_MODE dm, BUFFERED_LOGGING bl, const size_t userSize, const char *userFN, const uint16_t userBS)
{
    debugMode = dm;
    bufferedLogging = bl;
    userStackSize = userSize;
    userFileName = userFN;
    userBufferSize = userBS;
    resetTime();
    fileCheck();
    printSystemMessage(LoggerTags::INIT_MESSAGE);
    writeMessage(LoggerTags::INIT_MESSAGE);
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
    printSystemMessage(LoggerTags::BUFFERED_LOGGING_ON);
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
    printSystemMessage(LoggerTags::BUFFERED_LOGGING_OFF);
}

void Logger::info(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    handleMessage(LoggerTags::GREEN, MsgType::INFO, message, args);
    va_end(args);
}

void Logger::warn(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    handleMessage(LoggerTags::YELLOW, MsgType::WARN, message, args);
    va_end(args);
}

void Logger::error(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    handleMessage(LoggerTags::RED, MsgType::ERROR, message, args);
    va_end(args);
}

void Logger::debug(const char *message, ...)
{
    if (bufferedLogging == BUFFERED_LOGGING::OFF)
        return;

    va_list args;
    va_start(args, message);
    handleMessage(LoggerTags::WHITE, MsgType::DEBUG, message, args);
    va_end(args);
}

void Logger::handleMessage(const char *color, MsgType msgType, const char *message, va_list args)
{
    resetTime();
    va_list argsCopy;
    va_copy(argsCopy, args);
    State state = printMessage(color, msgType, message, argsCopy);
    va_end(argsCopy);
    if (state == State::FAILURE)
        return;

    std::unique_ptr<char[]> msg = createMessageForWrite(color, msgType, message, args);
    bufferedLogging == BUFFERED_LOGGING::ON ? stackMessage(std::move(msg), msgType) : writeMessage(msg.get());
}

void Logger::printSystemMessage(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    printMessage(LoggerTags::CYAN, MsgType::LOGGER, message, args);
    va_end(args);
}

State Logger::printMessage(const char *color, MsgType msgType, const char *message, va_list args)
{

    std::unique_ptr<char[]> msg = createMessageForPrint(color, msgType, message, args);

    if (msg == nullptr)
        return State::FAILURE;

    puts(msg.get());
    return State::SUCCESS;
}

std::unique_ptr<char[]> Logger::createMessageForPrint(const char *color, MsgType msgType, const char *message, va_list args)
{
    std::unique_ptr<char[]> buffer(new char[userBufferSize]);
    int formatCharCount = snprintf(buffer.get(), userBufferSize, "[%s]%s%s%s%s%s %s", std::strtok(ctime(&currentTime), "\n"), LoggerTags::CLR_ESC, color, getTypeString(msgType), LoggerTags::CLR_ESC, LoggerTags::RESET, message);

    if (hasBadFormat(formatCharCount, message))
        return nullptr;
    else if (isTooBig(formatCharCount, userBufferSize))
        printSystemMessage("Too big string. Saved only first %d characters from %zu", formatCharCount, userBufferSize);

    int withArgsCount = vsnprintf(buffer.get(), userBufferSize, buffer.get(), args);

    if (hasBadFormat(withArgsCount, message))
        return nullptr;
    else if (isTooBig(withArgsCount, userBufferSize))
        printSystemMessage("Too big string. Saved only first %d characters from %zu", withArgsCount, userBufferSize);

    return buffer;
}

bool Logger::hasBadFormat(int count, const char *message)
{
    if (count < 0)
    {
        printSystemMessage("Bad string format for [%s]", message);
        return true;
    }
    return false;
}

bool Logger::isTooBig(int count, size_t bufferSize)
{
    return count >= bufferSize;
}

std::unique_ptr<char[]> Logger::createMessageForWrite(const char *color, MsgType msgType, const char *message, va_list args)
{
    std::unique_ptr<char[]> buffer(new char[userBufferSize]);
    int formatCharCount = snprintf(buffer.get(), userBufferSize, "[%s]%s: %s", std::strtok(ctime(&currentTime), "\n"), getTypeString(msgType), message);
    int withArgsCount = vsnprintf(buffer.get(), userBufferSize, buffer.get(), args);

    std::unique_ptr<char[]> msg(new char[strlen(buffer.get()) + 1]);
    strcpy(msg.get(), buffer.get());
    return msg;
}

void Logger::flush()
{
    if (messageStack.empty())
    {
        printSystemMessage("flush: Trying to flush empty container.");
        return;
    }

    logFile = fopen(userFileName, "a");
    if (logFile == NULL)
    {
        printSystemMessage("writeMessage: Unable to write to file!");
        return;
    }
    std::for_each(begin(messageStack), end(messageStack), [](auto const &message) { write(message.get()); });
    fclose(logFile);
    messageStack.clear();
    messageStack.reserve(userStackSize);
}

void Logger::stackMessage(std::unique_ptr<char[]> message, MsgType msgType)
{
    if (messageStack.capacity() == 0)
    {
        if (debugMode == DEBUG_MODE::ON)
            printSystemMessage("stackMessage: Not enough capacity. Extending...");

        messageStack.reserve(userStackSize);
    }

    messageStack.emplace_back(std::move(message));

    if (messageStack.size() >= userStackSize || msgType == MsgType::ERROR)
    {
        if (debugMode == DEBUG_MODE::ON)
            printSystemMessage("stackMessage: Container full or message of type [ERROR] = flush()");

        flush();
    }
}

void Logger::resetTime()
{
    currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void Logger::writeMessage(const char *message)
{
    logFile = fopen(userFileName, "a");

    if (logFile == NULL)
    {
        printSystemMessage("writeMessage: Unable to write to file!");
        return;
    }
    write(message);
    fclose(logFile);
}

void Logger::write(const char *message)
{
    fputs(message, logFile);
    fputc('\n', logFile);
}

void Logger::finalCleanup()
{
    if (bufferedLogging == BUFFERED_LOGGING::ON && messageStack.empty())
    {
        flush();
    }
    printSystemMessage(LoggerTags::EXIT_MESSAGE);
    writeMessage(LoggerTags::EXIT_MESSAGE);
}

void Logger::fileCheck()
{
    logFile = fopen(userFileName, "a");
    if (logFile == NULL)
    {
        printSystemMessage("writeMessage: Unable to write to file!");
        return;
    }

    fclose(logFile);
}

const char *Logger::getTypeString(MsgType msgType)
{
    switch (msgType)
    {
    case MsgType::DEBUG:
        return LoggerTags::DEBUG;
    case MsgType::INFO:
        return LoggerTags::INFO;
    case MsgType::WARN:
        return LoggerTags::WARN;
    case MsgType::ERROR:
        return LoggerTags::ERROR;
    case MsgType::LOGGER:
        return LoggerTags::LOGGER;
    }
}