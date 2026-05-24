#include "../inc/string.hpp"

String::String() {
    buf_[0] = '\0';
    length = 0;
}

String::String(const char *string) {
    buf_[0] = '\0';
    length = 0;
    for (int i = 0; i < 16; i++) {
        char character = string[i];
        buf_[i] = character;
        if (character == '\0') {
            break;
        }
        length++;
    }
}

String::String(char string) {
    buf_[0] = string;
    length = 1;
}

String String::to_string(int value) {
    String valjund;
    for (int i = 16; i != 0 && value != 0; i--) {
        valjund = static_cast<String>("0123456789"[value % 10]) + valjund;
        value = value / 10;
    }
    return valjund;
}

String String::operator+(const String &string) {
    int new_size = length + string.length;
    if ( new_size > 16) {
        return String{"stfu"};
    }
   for (int i = length; i < new_size; i++) {
       buf_[i] = string.buf_[i-length];
   }
    length = new_size;
    return *this;
}

char String::operator[](int idx) const {
    if (idx >= length || idx < 0) {
        return '!';
    }
    return buf_[idx];
}