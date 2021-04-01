#ifndef LINE_H_
#define LINE_H_

#include <stdint.h>

typedef struct Line {
    uint32_t ch[1024];
    uint8_t len;
    uint8_t caret;
} Line;

Line* line_create(void);
void line_destroy(Line* line);
void line_show(Line* line);

void line_insert_char(Line* line, char c);
void line_delete_char(Line* line);

#endif