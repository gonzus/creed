#include <ctype.h>
#include <stdio.h>
#include <termbox.h>

#define TB_UNINITED -999

typedef struct Line {
    uint32_t ch[1024];
    uint8_t len;
    uint8_t caret;
} Line;

static Line line;

static int should_quit(struct tb_event *ev) {
    if (ev->key == TB_KEY_ENTER) return 1;
    if (ev->key == TB_KEY_CTRL_Q) return 1;
    if ((ev->ch == 'q' || ev->ch == 'Q') && (ev->mod & TB_MOD_ALT)) return 1;
    return 0;
}

static void insert_char(char c) {
    for (int j = line.len; j != line.caret; --j) {
        line.ch[j] = line.ch[j-1];
        tb_change_cell(j, 0, line.ch[j], TB_WHITE, TB_RED);
    }
    ++line.len;
    line.ch[line.caret] = c;
    tb_change_cell(line.caret, 0, c, TB_WHITE, TB_RED);
    ++line.caret;
}

static void dispatch_key_press(struct tb_event *ev)
{
    switch (ev->key) {
        case TB_KEY_ARROW_LEFT:
            if (line.caret > 0) {
                --line.caret;
            }
            break;

        case TB_KEY_ARROW_RIGHT:
            if (line.caret < line.len) {
                ++line.caret;
            }
            break;

        case TB_KEY_HOME:
            line.caret = 0;
            break;

        case TB_KEY_END:
            line.caret = line.len;
            break;

        case TB_KEY_DELETE:
            for (int j = line.caret; j < line.len; ++j) {
                line.ch[j] = line.ch[j+1];
                tb_change_cell(j, 0, line.ch[j+1], TB_WHITE, TB_RED);
            }
            tb_change_cell(line.len-1, 0, ' ', TB_DEFAULT, TB_DEFAULT);
            --line.len;
            break;

        case TB_KEY_SPACE:
            insert_char(' ');
            break;

        default:
            insert_char((char) ev->ch); // FIXME Unicode
            break;
    }
    tb_set_cursor(line.caret, 0);
}

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    int tb_ret = TB_UNINITED;

    do {
        tb_ret = tb_init();
        if (tb_ret < 0) {
            fprintf(stderr, "tb_init() failed with error code %d\n", tb_ret);
            break;
        }

        tb_select_input_mode(TB_INPUT_ALT | TB_INPUT_MOUSE);

        int done = 0;
        struct tb_event ev;
        tb_clear();
        tb_set_cursor(0, 0);
        while (tb_poll_event(&ev)) {
            switch (ev.type) {
                case TB_EVENT_KEY:
                    if (should_quit(&ev)) {
                        done = 1;
                        break;
                    }
                    dispatch_key_press(&ev);
                    break;
                case TB_EVENT_RESIZE:
                    break;
                case TB_EVENT_MOUSE:
                    break;
                default:
                    done = 1;
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

    printf("Line: %d chars, ", line.len);
    for (int j = 0; j < line.len; ++j) {
        printf("%c", j == 0 ? '[' : ':');
        if (line.ch[j] < 0xff) {
            char c = (char) line.ch[j];
            if (isprint(c)) {
                printf("%c", c);
            } else {
                printf("%x", (uint32_t) c);
            }
        } else {
            printf("%x", line.ch[j]);
        }
    }
    printf("], caret at %d\n", line.caret);
    return 0;
}
