#include <iostream>
#include <string.h>
#include "TextUtil.h"
using namespace std;

// copies text into a char array safely
void copyText(char dest[], const char src[], int size) {
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

// converts text to capital letters
void toUpperText(char text[]) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'a' && text[i] <= 'z')
            text[i] = text[i] - 32;
    }
}

// removes the enter at the end of a line
void removeNewline(char text[]) {
    int len = strlen(text);
    while (len > 0 && (text[len - 1] == '\n' || text[len - 1] == '\r')) {
        text[len - 1] = '\0';
        len--;
    }
}
