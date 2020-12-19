#pragma once

#include <string>

namespace ProgramNames
{
    inline static const std::string MENU = "MENU";
    inline static const std::string AUDIO = "AUDIO";
} // namespace ProgramNames

namespace Fonts
{
    inline static const int MEDIUM_FONT_SIZE = 28;
    inline constexpr char LAZY_FONT_PATH[] = "../assets/fonts/lazy.ttf";
} // namespace Fonts

namespace Programs
{
    namespace Menu
    {
        inline static const std::string SELECT_PROGRAM = "Select a program:";
    }
}

namespace LoggerTags
{
    inline constexpr char PATH[] = "log.txt";
    inline constexpr char ERROR[] = "[ERROR]";
    inline constexpr char INFO[] = "[INFO]";
    inline constexpr char WARN[] = "[WARN]";
    inline constexpr char LOGGER[] = "[LOGGER]";
    inline constexpr char CLR_ESC[] = "\033";
    inline constexpr char RED[] = "[0;31m";
    inline constexpr char YELLOW[] = "[0;33m";
    inline constexpr char GREEN[] = "[0;32m";
    inline constexpr char CYAN[] = "[0;36m";
    inline constexpr char RESET[] = "[0m";
    inline constexpr char ENDL[] = "\n";
    inline constexpr char INIT_MESSAGE[] = "LOGGING STARTED";
    inline constexpr char EXIT_MESSAGE[] = "LOGGING ENEDED";
    inline constexpr char DEBUG_MODE_ON[] = "DEBUG MODE:\t\t ON";
    inline constexpr char DEBUG_MODE_OFF[] = "DEBUG MODE:\t\t OFF";
    inline constexpr char BUFFERED_LOGGING_ON[] = "BUFFERED LOGGING:\t ON";
    inline constexpr char BUFFERED_LOGGING_OFF[] = "BUFFERED LOGGING:\t OFF";
} // namespace LoggerTags