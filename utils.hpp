#ifndef TINY_UTILS
#define TINY_UTILS

// built-in
#include <cstdint>
#include <string>
#include <vector>

// third-party
#include <fmt/format.h>


enum class Color
{
    FG_DEFAULT       [[maybe_unused]] = 39,
    FG_BLACK         [[maybe_unused]] = 30,
    FG_RED           [[maybe_unused]] = 31,
    FG_GREEN         [[maybe_unused]] = 32,
    FG_YELLOW        [[maybe_unused]] = 33,
    FG_BLUE          [[maybe_unused]] = 34,
    FG_MAGENTA       [[maybe_unused]] = 35,
    FG_CYAN          [[maybe_unused]] = 36,
    FG_LIGHT_GRAY    [[maybe_unused]] = 37,
    FG_DARK_GRAY     [[maybe_unused]] = 90,
    FG_LIGHT_RED     [[maybe_unused]] = 91,
    FG_LIGHT_GREEN   [[maybe_unused]] = 92,
    FG_LIGHT_YELLOW  [[maybe_unused]] = 93,
    FG_LIGHT_BLUE    [[maybe_unused]] = 94,
    FG_LIGHT_MAGENTA [[maybe_unused]] = 95,
    FG_LIGHT_CYAN    [[maybe_unused]] = 96,
    FG_WHITE         [[maybe_unused]] = 97,

    BG_RED           [[maybe_unused]] = 41,
    BG_GREEN         [[maybe_unused]] = 42,
    BG_BLUE          [[maybe_unused]] = 44,
    BG_DEFAULT       [[maybe_unused]] = 49
};

void panic(const char* const message)
{
    std::cout << message << std::endl;
    exit(1);
}

std::string get_color(Color md)
{
    std::string out {"\033["};
    out.append(std::to_string((uint32_t)md));
    out.append("m");
    return out;
}

struct Error
{
    bool error;
    uint32_t collum;

    std::string message;

    [[nodiscard]]
    std::string to_string() const
    {
        std::string error_type;

        if (this->error)
        {
            error_type = fmt::format("[{}{}{}]",
                get_color(Color::FG_LIGHT_RED),
                "ERROR",
                get_color(Color::FG_DEFAULT)
            );
        }
        else
        {
            error_type = fmt::format("[{}{}{}]",
                get_color(Color::FG_LIGHT_YELLOW),
                "WARNING",
                get_color(Color::FG_DEFAULT)
            );
        }

        return fmt::format("{}[{}]: {}", error_type, this->collum, this->message);
    }
};


class ErrorHandler
{

    std::vector<Error> error_list;

public:
    bool error;

    ErrorHandler(): error(false)
    {
        error_list.reserve(20);
    }

    void flush()
    {
        for (Error& er: this->error_list)
            std::cout << er.to_string() << '\n';

        this->error_list.clear();
        std::cout << std::flush;
    }


    void push_error(const char* const message, uint32_t collum)
    {
        this->error_list.push_back({true, collum, message});
        this->error = true;
    }

    void push_warning(const char* const message, uint32_t collum)
    {
        this->error_list.push_back({false, collum, message});
    }

};


#endif // TINY_UTILS
