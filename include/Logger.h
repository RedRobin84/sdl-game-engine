#pragma once

#include "Strings.h"
#include "Commons.h"

#include <vector>
#include <fstream>
#include <ctime>
#include <memory>

class Logger
{
public:

enum class DEBUG_MODE { OFF, ON };
enum class BUFFERED_LOGGING { OFF, ON };
enum class MsgType { LOGGER, INFO, WARN, ERROR, DEBUG };

   constexpr static size_t DEFAULT_STACK_SIZE = 10;
   constexpr static size_t DEFAULT_BUFFER_SIZE = 512;
   constexpr static char DEFAULT_FILE_NAME[] = "log.txt";

   static void init(DEBUG_MODE dm = DEBUG_MODE::OFF, BUFFERED_LOGGING bl = BUFFERED_LOGGING::OFF, 
                    const size_t userSize = DEFAULT_STACK_SIZE, const char * userFileName = DEFAULT_FILE_NAME,
                    const uint16_t userBufferSize = DEFAULT_BUFFER_SIZE);

   static void info(const char *message, ...);
   static void warn(const char *message, ...);
   static void error(const char *message, ...);
   static void debug(const char *message, ...);

   static void enableBufferedLogging();
   static void disableBufferedLogging();
   static void finalCleanup();

private:

   static std::vector<std::unique_ptr<char[]>> messageStack;
   static const char * userFileName;
   static time_t currentTime;
   static FILE * logFile;
   static BUFFERED_LOGGING bufferedLogging;
   static DEBUG_MODE debugMode;

   static size_t userStackSize;
   static uint16_t userBufferSize; 

   static void handleMessage(const char *color, MsgType msgType, const char *message, va_list args);
   static State printMessage(const char *color, MsgType msgType, const char *message, va_list args);
   static std::unique_ptr<char[]> createMessageForPrint(const char *color, MsgType msgType, const char *message, va_list args);
   static std::unique_ptr<char[]> createMessageForWrite(const char *color, MsgType msgType, const char *message, va_list args);
   static bool hasBadFormat(int count, const char *message);
   static bool isTooBig(int count, size_t bufferSize);
   static void printSystemMessage(const char *message, ...);
   static void flush();
   static void stackMessage(std::unique_ptr<char[]> message, MsgType msgType);
   static void resetTime();
   static void writeMessage(const char * message);
   static void write(const char * message);
   static void fileCheck();
   static const char * getTypeString(MsgType msgType);

   friend class LoggerTest;
};