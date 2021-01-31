// built-in
#include <iostream>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <unordered_map>

// local
#include "utils.hpp"
#include "globals.hpp"
#include "parser.hpp"



std::vector<Token> lex(const std::string& source, ErrorHandler& ec)
{

    static const std::unordered_map<char, TokenType> keymap
    {
        {'+', TokenType::PLUS},
        {'-', TokenType::MINUS},
        {'*', TokenType::STAR},
        {'/', TokenType::SLASH},
        {'(', TokenType::LPAREM},
        {')', TokenType::RPAREM}
    };

    uint32_t idx = 0;
    const uint32_t source_size = source.size();
    std::vector<Token> output;

    static const auto at_end = [&source_size, &idx]() -> bool
    {
        return idx >= source_size;
    };

    static const auto currchar = [&source, &idx](int32_t offset = 0) -> char
    {
        return source[idx + offset];
    };

    static const auto match = [](char ch, int32_t offset = 0) -> bool
    {
        return currchar(offset) == ch;
    };


    while (!at_end())
    {

        char ch = currchar();

        switch (ch) {

            case ' ':
            {
                do
                {
                    idx++;
                }
                while (!at_end() && match(' ', 0));
                break;
            }

            case '+':
            case '-':
            case '*':
            case '/':
            case '(':
            case ')':
            {
                output.emplace_back(keymap.at(ch), ++idx);
                break;
            }

            default:
            {
                if (isdigit(ch))
                {
                    uint32_t len = 0;
                    int64_t value = 0;

                    do
                    {
                        value = (value * 10) + (currchar() - '0');
                        idx++;
                        len++;
                    }
                    while (!at_end() && isdigit(currchar()));

                    if (match('.'))
                    {
                        const char* iptr = source.c_str() + idx;
                        char* end;

                        double fvalue = strtod(iptr, &end);
                        fvalue += value;

                        uint32_t flen = (uint64_t)end - (uint64_t)iptr;

                        len += flen;
                        idx += flen;

                        output.emplace_back(TokenType::FLOAT, (double)fvalue, (idx + 1 - len));
                    }
                    else
                        output.emplace_back(TokenType::INTEGER, (uint64_t)value, (idx + 1 - len));

                    break;
                }
                else
                {
                    ec.push_error("caracter inexperado", idx + 1);
                    idx++;
                }
            }
        }
    }

    output.emplace_back(TokenType::aEOF, idx + 1);
    return output;
}

int main() {

    ErrorHandler ec;

    while (true)
    {

        std::string source;

        std::cout << ">>> " << std::flush;
        std::getline(std::cin, source);

        if (std::cin.eof())
            break;

        auto lres = lex(source, ec);

        if (ec.error)
        {
            ec.flush();
            continue;
        }

        for (Token& tok: lres)
        {
            std::cout << TokenTypeRepr[(int)tok.type];
            if (tok.type == TokenType::INTEGER)
                std::cout << " " << tok.ivalue;
            else if(tok.type == TokenType::FLOAT)
                std::cout << " " << tok.fvalue;
            std::cout << '\n';
        }
        std::cout << std::flush;

        Eval ev {0.0, 0, EvalType::INTEGER};
        try
        {
            ev = parse(lres, ec);
        }
        catch (const ParserExcept& _) {}

        if (ec.error)
        {
            ec.flush();
            continue;
        }

        if (ev.type == EvalType::INTEGER)
            std::cout << ev.ivalue << std::endl;
        else if (ev.type == EvalType::FLOAT)
            std::cout << ev.fvalue << std::endl;

    }
    return 0;
}
