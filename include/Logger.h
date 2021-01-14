#pragma once

#include "Strings.h"
#include "Commons.h"

#include <cstdarg>
#include <vector>
#include <fstream>
#include <ctime>
#include <memory>

class Logger
{
public:

enum LoggerOptions : uint8_t
{
    LOGGER_DEFAULT = 0x00,
    WRITE_TO_FILE = 0x01,
    BUFFERED_LOGGING = 0x02,
    DEBUG_MODE = 0x04,
};

enum class MsgType { LOGGER, INFO, WARN, ERROR, DEBUG };

constexpr static size_t DEFAULT_STACK_SIZE = 10;
constexpr static size_t FLUSH_LOG_RESERVE = 2;
constexpr static size_t DEFAULT_BUFFER_SIZE = 512;
constexpr static char DEFAULT_FILE_NAME[] = "log.txt";

static void init(uint8_t options = LOGGER_DEFAULT, 
                 const char *userFileName = DEFAULT_FILE_NAME, 
                 const size_t userSize = DEFAULT_STACK_SIZE,
                 const uint16_t userBufferSize = DEFAULT_BUFFER_SIZE);

static void info(const char *message, ...);
static void warn(const char *message, ...);
static void error(const char *message, ...);
static void debug(const char *message, ...);
static void system(const char *message, ...);

static void enableBufferedLogging();
static void disableBufferedLogging();
static void finalCleanup();

private:
static std::vector<std::unique_ptr<char[]>> messageStack;
static const char *userFileName;
static time_t currentTime;
static FILE *logFile;
static uint8_t options;

static size_t userStackSize;
static uint16_t userBufferSize;

static uint8_t setOptions(uint8_t values);
static void handleMessage(MsgType msgType, const char *message, va_list args);
static State printMessage(MsgType msgType, const char *message);
static std::unique_ptr<const char[]> createMessageFromFormat(const char *message, va_list args);
static std::unique_ptr<const char[]> createMessageForPrint(MsgType msgType, const char *message);
static std::unique_ptr<char[]> createMessageForWrite(MsgType msgType, const char *message);
static std::unique_ptr<char[]> createNewStringFromBuffer(const char *buffer, size_t newSize);
static bool hasBadFormat(int count, const char *message);
static bool isTooBig(int count, size_t bufferSize);
static void flush();
static void stackMessage(std::unique_ptr<char[]> message, MsgType msgType);
static void resetTime();
static void writeMessage(const char *message);
static void write(const char *message);
static void fileCheck();
static const char *getTypeString(MsgType msgType);
static const char *getColorString(MsgType msgType);

friend class LoggerTest;
};