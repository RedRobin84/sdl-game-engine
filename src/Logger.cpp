#include "Logger.h"
#include "Strings.h"

#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <cstring>
#include <algorithm>
#include <chrono>

std::vector<std::unique_ptr<char[]>> Logger::messageStack;
time_t Logger::currentTime;
FILE *Logger::logFile;
const char *Logger::userFileName = DEFAULT_FILE_NAME;
uint8_t Logger::options = Logger::LOGGER_DEFAULT;
size_t Logger::userStackSize = DEFAULT_STACK_SIZE;
uint16_t Logger::userBufferSize = DEFAULT_BUFFER_SIZE;

enum class Logger::MsgType { LOGGER,
  INFO,
  WARN,
  ERROR,
  DEBUG };

void Logger::init(uint8_t opts, const char *userFN, const size_t userSize, const uint16_t userBuffSize)
{
  options = setOptions(opts);
  userFileName = userFN;
  userStackSize = userSize;
  userBufferSize = userBuffSize;
  resetTime();
  if ((options & WRITE_TO_FILE) != 0) {
    fileCheck();
  }
  printMessage(MsgType::LOGGER, LoggerTags::INIT_MESSAGE);
  if ((options & BUFFERED_LOGGING) != 0) {
    messageStack.reserve(userStackSize + FLUSH_LOG_RESERVE);
  }
  if ((options & WRITE_TO_FILE) != 0) {
    writeMessage(LoggerTags::INIT_MESSAGE);
  }
  debug((options & WRITE_TO_FILE) != 0 ? LoggerTags::WRITE_TO_FILE_ON : LoggerTags::WRITE_TO_FILE_OFF);
  debug((options & BUFFERED_LOGGING) != 0 ? LoggerTags::BUFFERED_LOGGING_ON : LoggerTags::BUFFERED_LOGGING_OFF);
  debug((options & DEBUG_MODE) != 0 ? LoggerTags::DEBUG_MODE_ON : LoggerTags::DEBUG_MODE_OFF);
}

uint8_t Logger::setOptions(uint8_t values)
{
  if (((values & WRITE_TO_FILE) == 0) && ((values & BUFFERED_LOGGING) != 0)) {
    warn("Logger::setOptions: Buffered logging is redundant while write to file is set off. Setting buffered logging off.");
    values &= ~BUFFERED_LOGGING;
  }
  return values;
}

void Logger::enableBufferedLogging()
{
  if ((options & BUFFERED_LOGGING) != 0) {
    debug("Logger::enableBufferedLogging: Buffered logging already on.");
    return;
  }

  options |= BUFFERED_LOGGING;
  messageStack.reserve(userStackSize + FLUSH_LOG_RESERVE);
  printf(LoggerTags::BUFFERED_LOGGING_ON);
}
void Logger::disableBufferedLogging()
{
  if ((options & BUFFERED_LOGGING) == 0) {
    debug("Logger::disableBufferedLogging: Buffered logging already off.");
    return;
  }

  options &= ~BUFFERED_LOGGING;
  flush();
  messageStack.clear();
  debug(LoggerTags::BUFFERED_LOGGING_OFF);
}

void Logger::info(const char *message, ...)
{
  va_list args;
  va_start(args, message);
  handleMessage(MsgType::INFO, message, args);
  va_end(args);
}

void Logger::warn(const char *message, ...)
{
  va_list args;
  va_start(args, message);
  handleMessage(MsgType::WARN, message, args);
  va_end(args);
}

void Logger::error(const char *message, ...)
{
  va_list args;
  va_start(args, message);
  handleMessage(MsgType::ERROR, message, args);
  va_end(args);
}

void Logger::debug(const char *message, ...)
{
  if ((options & BUFFERED_LOGGING) == 0) {
    return;
  }

  va_list args;
  va_start(args, message);
  handleMessage(MsgType::DEBUG, message, args);
  va_end(args);
}

void Logger::system(const char *message, ...)
{
  va_list args;
  va_start(args, message);
  handleMessage(MsgType::LOGGER, message, args);
  va_end(args);
}

void Logger::handleMessage(MsgType msgType, const char *message, va_list args)
{
  resetTime();
  std::unique_ptr<const char[]> formattedMessage = createMessageFromFormat(message, args);
  if (formattedMessage == nullptr) {
    return;
  }

  State state = printMessage(msgType, formattedMessage.get());
  if (state == State::FAILURE) {
    return;
  }

  std::unique_ptr<char[]> msg = createMessageForWrite(msgType, formattedMessage.get());

  if ((options & WRITE_TO_FILE) != 0) {
    (options & BUFFERED_LOGGING) != 0 ? stackMessage(std::move(msg), msgType) : writeMessage(msg.get());
  }
}

State Logger::printMessage(MsgType msgType, const char *message)
{
  std::unique_ptr<const char[]> msg = createMessageForPrint(msgType, message);

  if (msg == nullptr) {
    return State::FAILURE;
  }

  puts(msg.get());
  return State::SUCCESS;
}

std::unique_ptr<const char[]> Logger::createMessageFromFormat(const char *message, va_list args)
{
  char buffer[userBufferSize];
  int formatCharCount = vsnprintf(buffer, sizeof buffer, message, args);

  if (hasBadFormat(formatCharCount, message)) {
    return nullptr;
  }
  if (isTooBig(formatCharCount, sizeof buffer)) {
    warn("Logger::createMessageFromFormat: Too big string. Saved only first %zu characters from %zu", sizeof buffer, strlen(message));
  }

  std::unique_ptr<char[]> msg = createNewStringFromBuffer(buffer, strlen(buffer) + 1);
  return msg;
}

std::unique_ptr<const char[]> Logger::createMessageForPrint(MsgType msgType, const char *message)
{
  char buffer[userBufferSize];
  int formatCharCount = snprintf(buffer, sizeof buffer, "[%s]%s%s%s%s%s %s", std::strtok(ctime(&currentTime), "\n"), LoggerTags::CLR_ESC, getColorString(msgType), getTypeString(msgType), LoggerTags::CLR_ESC, LoggerTags::RESET, message);

  if (hasBadFormat(formatCharCount, message)) {
    return nullptr;
  }
  if (isTooBig(formatCharCount, sizeof buffer)) {
    warn("Logger::createMessageForPrint: Too big string. Saved only first %d characters from %zu", sizeof buffer, strlen(message));
  }

  std::unique_ptr<char[]> msg = createNewStringFromBuffer(buffer, strlen(buffer) + 1);
  return msg;
}

std::unique_ptr<char[]> Logger::createNewStringFromBuffer(const char *buffer, size_t newSize)
{
  std::unique_ptr<char[]> msg(new char[newSize]);
  strcpy(msg.get(), buffer);
  return msg;
}

bool Logger::hasBadFormat(int count, const char *message)
{
  if (count < 0) {
    error("Logger::hasBadFirnat: Bad string format for [%s]", message);
    return true;
  }
  return false;
}

bool Logger::isTooBig(int count, int bufferSize)
{
  return count >= bufferSize;
}

std::unique_ptr<char[]> Logger::createMessageForWrite(MsgType msgType, const char *message)
{
  char buffer[userBufferSize];
  snprintf(buffer, sizeof buffer, "[%s]%s: %s", std::strtok(ctime(&currentTime), "\n"), getTypeString(msgType), message);

  std::unique_ptr<char[]> msg = createNewStringFromBuffer(buffer, strlen(buffer) + 1);
  return msg;
}

void Logger::flush()
{
  if (messageStack.empty()) {
    warn("Logger::flush: Trying to flush empty container.");
    return;
  }

  logFile = fopen(userFileName, "a");
  if (logFile == NULL) {
    error("Logger::writeMessage: Unable to write to file!");
    return;
  }
  std::for_each(begin(messageStack), end(messageStack), [](auto const &message) { write(message.get()); });
  fclose(logFile);
  messageStack.clear();
  messageStack.reserve(userStackSize + FLUSH_LOG_RESERVE);
}

void Logger::stackMessage(std::unique_ptr<char[]> message, MsgType msgType)
{
  if (messageStack.capacity() == 0) {
    messageStack.reserve(userStackSize + FLUSH_LOG_RESERVE);

    if ((options & DEBUG_MODE) != 0)
      debug("Logger::stackMessage: Not enough capacity. Extending...");
  }

  messageStack.emplace_back(std::move(message));

  if (messageStack.size() >= userStackSize || msgType == MsgType::ERROR) {
    if ((options & DEBUG_MODE) != 0)
      debug("Logger::stackMessage: Container full or message of type [ERROR] = flush()");

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

  if (logFile == NULL) {
    error("Logger::writeMessage: Unable to write to file!");
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
  if (((options & BUFFERED_LOGGING) != 0) && !messageStack.empty()) {
    flush();
  }
  printMessage(MsgType::LOGGER, LoggerTags::EXIT_MESSAGE);
  writeMessage(LoggerTags::EXIT_MESSAGE);
  writeMessage(" ");
}

void Logger::fileCheck()
{
  logFile = fopen(userFileName, "a");
  if (logFile == NULL) {
    error("Logger::writeMessage: Unable to write to file!");
    return;
  }

  fclose(logFile);
}

const char *Logger::getTypeString(MsgType msgType)
{
  switch (msgType) {
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
  default:
    throw std::runtime_error("Logger::getTypeString: Unknown MsgType.");
  }
}

const char *Logger::getColorString(MsgType msgType)
{
  switch (msgType) {
  case MsgType::DEBUG:
    return LoggerTags::MAGENTA;
  case MsgType::INFO:
    return LoggerTags::GREEN;
  case MsgType::WARN:
    return LoggerTags::YELLOW;
  case MsgType::ERROR:
    return LoggerTags::RED;
  case MsgType::LOGGER:
    return LoggerTags::CYAN;
  default:
    throw std::runtime_error("Logger::getColorString: Unknown MsgType.");
  }
}