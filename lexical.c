#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

bool isKeyword(char buffer[]) {
    char keywords[32][10] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
    };

    for (int i = 0; i < 32; ++i) {
        if (strcmp(keywords[i], buffer) == 0) {
            return true;
        }
    }
    return false;
}

bool isOperator(char ch) {
    char operators[] = "+-*/%=><!";
    for (int i = 0; i < 9; i++) {
        if (ch == operators[i])
            return true;
    }
    return false;
}

bool isSpecial(char ch) {
    char specials[] = "();{}[]";
    for (int i = 0; i < 7; i++) {
        if (ch == specials[i])
            return true;
    }
    return false;
}

bool isNumber(char buffer[]) {
    int len = strlen(buffer);
    for (int i = 0; i < len; i++) {
        if (!isdigit(buffer[i]) && buffer[i] != '.')
            return false;
    }
    return true;
}

int main() {
    char ch, buffer[15];
    FILE *fp;
    int j = 0;
    fp = fopen("lex.txt", "r");
    if (fp == NULL) {
        printf("Error while opening the file\n");
        return 1;
    }
    
    while ((ch = fgetc(fp)) != EOF) {
        if (isalnum(ch) || ch == '.') {
            buffer[j++] = ch;
        } else if ((ch == ' ' || ch == '\n' || isOperator(ch) || isSpecial(ch)) && (j != 0)) {
            buffer[j] = '\0';
            j = 0;
            if (isNumber(buffer))
                printf("[%s : Number]\n", buffer);
            else if (isKeyword(buffer))
                printf("[%s : Keyword]\n", buffer);
            else
                printf("[%s : Identifier]\n", buffer);
        }

        // Handle single-line comments
        if (ch == '/') {
            char next = fgetc(fp);
            if (next == '/') {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                continue;
            } else if (next == '*') {
                while (true) {
                    ch = fgetc(fp);
                    if (ch == EOF) break;
                    if (ch == '*') {
                        if ((ch = fgetc(fp)) == '/') break;
                    }
                }
                continue;
            } else {
                ungetc(next, fp); // Put back the character if it's not part of a comment
            }
        }

        if (isOperator(ch))
            printf("[%c : Operator]\n", ch);
        else if (isSpecial(ch))
            printf("[%c : Special Symbol]\n", ch);
    }
    
    fclose(fp);
    return 0;
}
