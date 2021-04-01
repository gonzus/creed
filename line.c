#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termbox.h>
#include "line.h"

Line* line_create(void) {
    Line* line = (Line*) malloc(sizeof(Line));
    memset(line, 0, sizeof(Line));
    return line;
}

void line_destroy(Line* line) {
    free((void*) line);
}

void line_show(Line* line) {
    printf("Line: %d chars, ", line->len);
    for (int j = 0; j < line->len; ++j) {
        printf("%c", j == 0 ? '[' : ':');
        if (line->ch[j] < 0xff) {
            char c = (char) line->ch[j];
            if (isprint(c)) {
                printf("%c", c);
            } else {
                printf("%x", (uint32_t) c);
            }
        } else {
            printf("%x", line->ch[j]);
        }
    }
    printf("], caret at %d\n", line->caret);
}

void line_insert_char(Line* line, char c) {
    for (int j = line->len; j != line->caret; --j) {
        line->ch[j] = line->ch[j-1];
        tb_change_cell(j, 0, line->ch[j], TB_WHITE, TB_RED);
    }
    ++line->len;
    line->ch[line->caret] = c;
    tb_change_cell(line->caret, 0, c, TB_WHITE, TB_RED);
    ++line->caret;
}

void line_delete_char(Line* line) {
    for (int j = line->caret; j < line->len; ++j) {
        line->ch[j] = line->ch[j+1];
        tb_change_cell(j, 0, line->ch[j+1], TB_WHITE, TB_RED);
    }
    tb_change_cell(line->len-1, 0, ' ', TB_DEFAULT, TB_DEFAULT);
    --line->len;
}
