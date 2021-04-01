#include <stdio.h>
#include <stdbool.h>
#include <termbox.h>
#include "line.h"

#define TB_UNINITED -999

static Line* line;

static bool dispatch_key_press(struct tb_event *ev)
{
    switch (ev->key) {
        case TB_KEY_ENTER:
        case TB_KEY_CTRL_Q:
            return true;

        case TB_KEY_ARROW_LEFT:
#if 0
            if (ev->mod & TB_MOD_ALT) {
                printf("%s!!!\n", "ALT");
            }
#endif
            if (line->caret > 0) {
                --line->caret;
            }
            break;

        case TB_KEY_ARROW_RIGHT:
            if (line->caret < line->len) {
                ++line->caret;
            }
            break;

        case TB_KEY_HOME:
            line->caret = 0;
            break;

        case TB_KEY_END:
            line->caret = line->len;
            break;

        case TB_KEY_DELETE:
            line_delete_char(line);
            break;

        case TB_KEY_SPACE:
            line_insert_char(line, ' '); // FIXME Unicode
            break;

        default:
            if ((ev->ch == 'q' || ev->ch == 'Q') &&
                (ev->mod & TB_MOD_ALT)) {
                return 1;
            }
            line_insert_char(line, (char) ev->ch); // FIXME Unicode
            break;
    }
    tb_set_cursor(line->caret, 0);
    return false;
}

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    int tb_ret = TB_UNINITED;

    do {
        line = line_create();
        if (!line) {
            break;
        }
        tb_ret = tb_init();
        if (tb_ret < 0) {
            fprintf(stderr, "tb_init() failed with error code %d\n", tb_ret);
            break;
        }

        tb_select_input_mode(TB_INPUT_ALT | TB_INPUT_MOUSE);

        bool done = false;
        struct tb_event ev;
        tb_clear();
        tb_set_cursor(0, 0);
        while (tb_poll_event(&ev)) {
            switch (ev.type) {
                case TB_EVENT_KEY:
                    done = dispatch_key_press(&ev);
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
    } while (0);

    if (tb_ret >= 0) {
        tb_shutdown();
        tb_ret = TB_UNINITED;
    }

    if (line) {
        line_show(line);
        line_destroy(line);
        line = 0;
    }

    return 0;
}
