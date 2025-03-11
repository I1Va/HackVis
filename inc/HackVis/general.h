#ifndef GENERAL_H
#define GENERAL_H

#include <stddef.h>

#define GRN "\e[0;32m"
#define WHT "\e[0;20m"
#define RED "\e[0;31m"
#define YEL "\e[0;33m"

#define PRINTF_RED(str_, ...) printf(RED str_ WHT, ##__VA_ARGS__)
#define PRINTF_WHT(str_, ...) printf(WHT str_ WHT, ##__VA_ARGS__)
#define PRINTF_GRN(str_, ...) printf(GRN str_ WHT, ##__VA_ARGS__)
#define PRINTF_YEL(str_, ...) printf(YEL str_ WHT, ##__VA_ARGS__)

#define FPRINTF_RED(stream, str_, ...) fprintf(stream, RED str_ WHT, ##__VA_ARGS__)
#define FPRINTF_WHT(stream, str_, ...) fprintf(stream, WHT str_ WHT, ##__VA_ARGS__)
#define FPRINTF_GRN(stream, str_, ...) fprintf(stream, GRN str_ WHT, ##__VA_ARGS__)
#define FPRINTF_YEL(stream, str_, ...) fprintf(stream, YEL str_ WHT, ##__VA_ARGS__)

#define GENERAL_ASSERT(error, end_instruction)                                                                                    \
    FPRINTF_RED(stderr, "{%s} [%s: %d]: descr{%s}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, hackvis_get_descr(error));    \
    end_instruction;

#define CLEAR_MEMORY(mark) goto mark;

#ifdef _DEBUG
    #define debug(str_, ...) FPRINTF_RED(stderr, "{%s} [%s: %d]: descr{" str_ "}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
    #define DEBUG_ERROR(code) FPRINTF_RED(stderr, "{%s} [%s: %d]: descr{%s}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, get_bit_descr(code));
    #define ON_DEBUG(...) __VA_ARGS__

    #define MY_ASSERT(err_code, exit_instruction) {DEBUG_ERROR(err_code); exit_instruction;};

#else
    #define debug(str_, ...) ;
    #define DEBUG_ERROR(code) ;
    #define ON_DEBUG(...)
    #define MY_ASSERT(err_code, exit_instruction) ;

#endif // _DEBUG

#endif // GENERAL