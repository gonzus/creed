#ifndef CREED_H_
#define CREED_H_

struct Line;

typedef struct Creed {
    struct Line* line;
} Creed;

Creed* creed_create(void);
void creed_destroy(Creed* creed);
void creed_run(Creed* creed);

#endif