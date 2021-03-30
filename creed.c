#include <stdio.h>
#include <termbox.h>

#define TB_UNINITED -999

typedef struct Line {
    uint32_t ch[1024];
    uint8_t len;
    uint8_t caret;
} Line;

static Line line;

static void dispatch_key_press(struct tb_event *ev)
{
    line.ch[line.len++] = ev->ch;
    tb_change_cell(line.caret, 0, ev->ch, TB_WHITE, TB_RED);
    line.caret++;
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

        tb_select_input_mode(TB_INPUT_ESC | TB_INPUT_MOUSE);

        int done = 0;
        struct tb_event ev;
        tb_clear();
        while (tb_poll_event(&ev)) {
            switch (ev.type) {
                case TB_EVENT_KEY:
                    if (ev.key == TB_KEY_CTRL_Q) {
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
        printf("%c%x", j == 0 ? '[' : ':', line.ch[j]);
    }
    printf("], caret at %d\n", line.caret);
    return 0;
}
