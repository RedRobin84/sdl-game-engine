#pragma once

#include "Strings.h"

#include <vector>
#include <fstream>
#include <ctime>

class Logger
{
public:

enum class DEBUG_MODE { OFF, ON };
enum class BUFFERED_LOGGING { OFF, ON };

   constexpr static size_t DEFAULT_STACK_SIZE = 10;
   constexpr static char DEFAULT_FILE_NAME[] = "log.txt";

   static void init(DEBUG_MODE dm = DEBUG_MODE::OFF, BUFFERED_LOGGING bl = BUFFERED_LOGGING::OFF, 
                    const size_t userSize = DEFAULT_STACK_SIZE, const char * userFileName = DEFAULT_FILE_NAME);

   static void info(const char *message);
   static void warn(const char *message);
   static void error(const char *message);
   static void debug(const char *message);

   static void enableBufferedLogging();
   static void disableBufferedLogging();
   static void finalCleanup();

private:
   struct Message
   {
      const char *message;
      const char *msgType;
      time_t msgTime;

      Message(const char *msg, const char *messageType, time_t messageTime)
          : message(msg), msgType(messageType), msgTime(messageTime) {}
   };

   static std::vector<Message> messageStack;
   static const char * userFileName;
   static time_t currentTime;
   static std::ofstream logFile;
   static BUFFERED_LOGGING bufferedLogging;
   static DEBUG_MODE debugMode;

   static size_t userStackSize; 

   static void handleMessage(const char *color, const char *typeHeader, const char *message);
   static void printMessage(const char *color, const char *typeHeader, const char *message);
   static void printSystemMessage(const char *message);
   static void flush();
   static void stackMessage(const char *message, const char *typeHeader);
   static void resetTime();
   static void writeMessage(const char *message, const char *typeHeader);
   static void write(const char *message, const char *typeHeader, time_t time = 0L);
   static void fileCheck();

   friend class LoggerTest;
};