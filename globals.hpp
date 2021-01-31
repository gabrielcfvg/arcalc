
#ifndef ARCALC_GLOBALS_HPP
#define ARCALC_GLOBALS_HPP


// third-party
#include <fmt/format.h>

// local
#include "utils.hpp"


enum class TokenType
{
    PLUS,
    MINUS,
    STAR,
    SLASH,
    LPAREM,
    RPAREM,
    INTEGER,
    FLOAT,
    aEOF
};

const char* const TokenTypeRepr[]
{
    "PLUS",
    "MINUS",
    "STAR",
    "SLASH",
    "LPAREM",
    "RPAREM",
    "INTEGER",
    "FLOAT",
    "EOF"
};

struct Token
{
    TokenType type;
    int64_t  ivalue;
    double    fvalue;
    uint32_t  collum;

    Token(TokenType t, uint32_t c)
    : type(t), ivalue(0), fvalue(0), collum(c) {}

    Token(TokenType t, uint64_t i, uint32_t c)
    : type(t), ivalue(i), fvalue(0), collum(c) {}

    Token(TokenType t, double f, uint32_t c)
    : type(t), ivalue(0), fvalue(f), collum(c) {}
};

enum class EvalType
{
    INTEGER,
    FLOAT
};

struct Eval
{
    double   fvalue;
    int64_t  ivalue;
    EvalType type;

    void convert(EvalType type)
    {
        if (type != this->type)
        {
            if (type == EvalType::INTEGER && this->type == EvalType::FLOAT)
                this->ivalue = (int64_t)this->fvalue;
            else if (type == EvalType::FLOAT && this->type == EvalType::INTEGER)
                this->fvalue = (double)this->ivalue;

            this->type = type;
        }
    }
};

#endif //ARCALC_GLOBALS_HPP
