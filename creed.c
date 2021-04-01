#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termbox.h>
#include "line.h"
#include "creed.h"

static bool dispatch_key_press(Creed* creed, struct tb_event *ev);

Creed* creed_create(void) {
    if (tb_init() < 0) {
        fprintf(stderr, "tb_init() failed\n");
        return 0;
    }
    tb_select_input_mode(TB_INPUT_ALT | TB_INPUT_MOUSE);

    Creed* creed = (Creed*) malloc(sizeof(Creed));
    memset(creed, 0, sizeof(Creed));
    creed->line = line_create();

    return creed;
}

void creed_destroy(Creed* creed) {
    tb_shutdown();
    line_show(creed->line);
    line_destroy(creed->line);
    free((void*) creed);
}

void creed_run(Creed* creed) {
    tb_clear();
    tb_set_cursor(0, 0);
    bool done = false;
    struct tb_event ev;
    while (tb_poll_event(&ev)) {
        switch (ev.type) {
            case TB_EVENT_KEY:
                done = dispatch_key_press(creed, &ev);
                break;
            case TB_EVENT_RESIZE:
                break;
            case TB_EVENT_MOUSE:
                break;
            default:
                done = true;
                break;
        }
        if (done) {
            break;
        }
        tb_present();
    }
}

static bool dispatch_key_press(Creed* creed, struct tb_event *ev) {
    switch (ev->key) {
        case TB_KEY_ENTER:
        case TB_KEY_CTRL_Q:
            return true;

        case TB_KEY_ARROW_LEFT:
            if (creed->line->caret > 0) {
                --creed->line->caret;
            }
            break;

        case TB_KEY_ARROW_RIGHT:
            if (creed->line->caret < creed->line->len) {
                ++creed->line->caret;
            }
            break;

        case TB_KEY_HOME:
            creed->line->caret = 0;
            break;

        case TB_KEY_END:
            creed->line->caret = creed->line->len;
            break;

        case TB_KEY_DELETE:
            line_delete_char(creed->line);
            break;

        case TB_KEY_SPACE:
            line_insert_char(creed->line, ' '); // FIXME Unicode
            break;

        default:
            if ((ev->ch == 'q' || ev->ch == 'Q') &&
                (ev->mod & TB_MOD_ALT)) {
                return 1;
            }
            line_insert_char(creed->line, (char) ev->ch); // FIXME Unicode
            break;
    }
    tb_set_cursor(creed->line->caret, 0);
    return false;
}
