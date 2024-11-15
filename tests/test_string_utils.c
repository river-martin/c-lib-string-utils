#include "../include/str_to_argv.h"
#include <stdio.h>

int main(int argc, const char **argv) {
    int argc_out;
    char **argv_out = str_to_argv("./bin/bru \"(a|b| |c)\" asdf1234", &argc_out);
    for (int i = 0; i < argc_out; i++) {
        printf("%s\n", argv_out[i]);
    }
    return 0;
}
