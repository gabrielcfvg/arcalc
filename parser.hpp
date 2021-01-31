#ifndef ARCALC_PARSER_HPP
#define ARCALC_PARSER_HPP

// built-in
#include <exception>
#include <unordered_set>

// local
#include "globals.hpp"




class ParserExcept: public std::exception
{
    public:

        [[nodiscard]]
        const char* what() const noexcept override
        {
            return "parser fatal exception";
        }
};


struct ParseData
{
    const std::vector<Token>& input;
    ErrorHandler& ec;

    uint32_t idx;
    uint32_t input_size;

    ParseData(const std::vector<Token>& i, ErrorHandler& ec): input(i), ec(ec)
    {
        this->idx = 0;
        this->input_size = this->input.size();
    }

    [[nodiscard]]
    bool at_end(int32_t offset = 0) const
    {
        return (this->idx + offset) >= this->input_size;
    }

    [[nodiscard]]
    const Token& currtok(int32_t offset = 0) const
    {
        return this->input[this->idx + offset];
    }

    [[nodiscard]]
    bool match(TokenType type, int32_t offset = 0) const
    {
        return this->currtok(offset).type == type;
    }

    void advance()
    {
        this->idx++;
    }
};


Eval term(ParseData& pd);
Eval factor(ParseData& pd);
Eval unary(ParseData& pd);
Eval value(ParseData& pd);

Eval parse(const std::vector<Token>& input, ErrorHandler& ec)
{

    auto data = ParseData{input, ec};

    return term(data);
}

Eval term(ParseData& pd)
{

    Eval ev = factor(pd);

    while (pd.match(TokenType::MINUS) || pd.match(TokenType::PLUS))
    {
        const Token& oprt = pd.currtok();
        pd.advance();

        Eval ev2 = term(pd);

        if (ev.type == EvalType::FLOAT || ev2.type == EvalType::FLOAT)
        {
            ev.convert(EvalType::FLOAT);
            ev2.convert(EvalType::FLOAT);
        }

        if (oprt.type == TokenType::MINUS)
        {
            ev.fvalue -= ev2.fvalue;
            ev.ivalue -= ev2.ivalue;
        }
        else if (oprt.type == TokenType::PLUS)
        {
            ev.fvalue += ev2.fvalue;
            ev.ivalue += ev2.ivalue;
        }
    }

    return ev;
}

Eval factor(ParseData& pd)
{

    const auto zerodiv = [&pd](auto v)
    {
        if (v == 0.0)
        {
            pd.ec.push_error("divisão por 0", 0);
            throw ParserExcept{};
        }
    };

    Eval ev = unary(pd);

    while (pd.match(TokenType::SLASH) || pd.match(TokenType::STAR))
    {
        const Token& oprt = pd.currtok();
        pd.advance();

        Eval ev2 = unary(pd);

        if (ev.type == EvalType::FLOAT || ev2.type == EvalType::FLOAT)
        {
            ev.convert(EvalType::FLOAT);
            ev2.convert(EvalType::FLOAT);
        }

        if (oprt.type == TokenType::STAR)
        {
            if (ev.type == EvalType::FLOAT)
                ev.fvalue *= ev2.fvalue;
            if (ev.type == EvalType::INTEGER)
                ev.ivalue *= ev2.ivalue;
        }
        else if (oprt.type == TokenType::SLASH)
        {
            if (ev.type == EvalType::FLOAT)
            {
                zerodiv(ev2.fvalue);
                ev.fvalue /= ev2.fvalue;
            }
            if (ev.type == EvalType::INTEGER)
            {
                zerodiv(ev2.ivalue);
                ev.ivalue /= ev2.ivalue;
            }
        }
    }

    return ev;
}

Eval unary(ParseData& pd)
{

    if (pd.match(TokenType::PLUS))
    {
        pd.advance();
        return value(pd);
    }
    if (pd.match(TokenType::MINUS))
    {
        pd.advance();
        Eval ev = unary(pd);

        ev.fvalue *= -1;
        ev.ivalue *= -1;
        return ev;
    }

    return value(pd);
}

Eval value(ParseData& pd)
{
    if (pd.at_end())
    {
        uint32_t col = 0;
        if (!pd.at_end(-1))
            col = pd.currtok(-1).collum;

        pd.ec.push_error("expressão mal formada", col);
        throw ParserExcept{};
    }

    const Token &tk = pd.currtok();
    pd.advance();

    Eval ev {0.0, 0, EvalType::INTEGER};

    if (tk.type == TokenType::INTEGER)
        ev.ivalue = tk.ivalue;
    else if (tk.type == TokenType::FLOAT)
    {
        ev.type = EvalType::FLOAT;
        ev.fvalue = tk.fvalue;
    }
    else if (tk.type == TokenType::LPAREM)
    {
        ev = term(pd);
        if (pd.match(TokenType::RPAREM))
            pd.advance();
        else
        {
            uint32_t col = 0;
            if (!pd.at_end(-1))
                col = pd.currtok(-1).collum;

            pd.ec.push_error("expressão mal formada", col);
            throw ParserExcept{};
        }
    }
    else
    {
        pd.ec.push_error("token inesperado", tk.collum);
        throw ParserExcept{};
    }

    return ev;
}




#endif //ARCALC_PARSER_HPP
