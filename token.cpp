//
// Created by david on 3/21/18.
//

#include "token.h"

#include <cctype>
#include <cstring>

static inline bool isBlank(char c) {
    return ((c == ' ') | (c == '\t'));
}

