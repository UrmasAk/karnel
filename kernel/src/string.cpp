//
// Created by jesper on 24.05.26.
//

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

String String::toString(int value) {
    String valjund;
    
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