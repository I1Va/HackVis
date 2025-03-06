#ifndef BIN_PATCHER_H

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "bin_patcher_errproc.h"

const unsigned long long HASH_MULT = 131;
const unsigned long long CRACKME_HASH = 389332;
const size_t             PATHED_WORD_ADDR = 0x11;

int get_file_sz(const char *const path);
unsigned long long get_hash(const char *string, const size_t len);
bool proccess_crackme_file(FILE *file_ptr, size_t file_sz, parser_err *err=NULL);

#endif // BIN_PATCHER_H
