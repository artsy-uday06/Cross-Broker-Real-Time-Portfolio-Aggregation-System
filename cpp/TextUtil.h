#ifndef TEXTUTIL_H
#define TEXTUTIL_H

const int TEXT_LEN = 30;
const int NAME_LEN = 60;
const int DATE_LEN = 12;

void copyText(char dest[], const char src[], int size);

void toUpperText(char text[]);

void removeNewline(char text[]);
#endif
