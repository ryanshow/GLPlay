#pragma once

#include <fmt/format.h>
#include <cstdlib>

namespace GLPlay {

template <typename... T>
void Exit(const char* format, const T & ... args) {
    fmt::print(stderr, format, args...);
    fmt::print(stderr, "\n");
    std::exit(EXIT_FAILURE);
}

template <typename T, typename... U>
void ExitCheck(T ret_code, const char * format, const U & ... args) {
    if (!ret_code) {
        Exit(format, args...);
    }
};

unsigned char *CreateTextureAtlas(unsigned char *font_buffer, float size);

}