#ifndef BIN_PATCHER_ERRPROC
#define BIN_PATCHER_ERRPROC

enum parser_err {
    PARSER_OK,
    PARSER_FILE_OPEN_ERR,
    PARSER_CALLOC,
    PARSER_UNKNOWN_COM_CODE,
};

#endif // BIN_PATCHER_ERRPROC