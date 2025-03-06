#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include "general.h"
#include "bin_patcher.h"

int get_file_sz(const char *const path) {
    struct stat buf = {};
    int stat_ret = stat(path, &buf);

    if (stat_ret != 0) {
        debug("stat error code : {%d}", stat_ret);
        return -1;
    }

    return buf.st_size;
}

unsigned long long get_hash(const char *string, const size_t len) {
    unsigned long long hash = 0;
    for (size_t i = 0; i < len; i++) {
        hash = hash + string[i] * HASH_MULT;
    }
    return hash;
}

bool proccess_crackme_file(FILE *file_ptr, size_t file_sz, parser_err *err) {
    char *byte_file = (char *) calloc(file_sz, sizeof(char));
    if (!byte_file) {
        debug("calloc failed");
        if (err) { *err = PARSER_CALLOC; };
    }

    fread(byte_file, sizeof(char), file_sz, file_ptr);

    unsigned long long hash = get_hash(byte_file, file_sz);
    if (hash != CRACKME_HASH) {
        printf("The file selected is not suitable for hacking\n");
        return false;
    }

    byte_file[PATHED_WORD_ADDR] = 0x3B;
    byte_file[PATHED_WORD_ADDR + 1] = 0xC0;

    FILE *new_file = fopen("crackedfile.com", "wb"); // FIXME:
    size_t count = fwrite(byte_file, sizeof(char), file_sz, new_file);

    printf("HACK IS DONE");
    return true;
}