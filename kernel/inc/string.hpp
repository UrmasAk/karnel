#pragma once

#include "cstddef"
#include "cstdint"


class String {
    char buf_[16]{};
public:
    uint8_t length{};
    String();
    String(const char *string);
    String(char string);
    static String to_string(int value);

    String operator+(const String &string);

    char operator[](int idx) const;
};
