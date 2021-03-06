#ifndef ark_repl
#define ark_repl

#include <iostream>
#include <sstream>

#include <Ark/Constants.hpp>
#include <Ark/Compiler/Compiler.hpp>
#include <Ark/VM/VM.hpp>
#include <Ark/VM/State.hpp>

namespace Ark
{
    class Repl
    {
    public:
        /**
         * @brief Construct a new Repl object
         * 
         * @param options the REPL options
         * @param lib_dir the path to the standard library
         */
        Repl(uint16_t options, std::string lib_dir);

        /**
         * @brief Start the REPL
         * 
         */
        void run();

    private:
        std::string m_lib_dir;
        uint16_t m_options;

        inline void print_repl_header();
        int count_open_parentheses(const std::string& line);
        int count_open_braces(const std::string& line);
        void trim_whitespace(std::string& line);
    };
}

#endif