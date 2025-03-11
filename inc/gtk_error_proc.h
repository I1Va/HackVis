
#ifndef GTK_ERROR_PROC_H
#define GTK_ERROR_PROC_H

#include "general.h"

enum gtk_err
{
    GTK_OK                      = 0,
    GTK_OVERLAY_NEW_ERR         = 1,
    GTK_BOX_NEW_ERR             = 2,
    GTK_BUTTON_NEW_ERR          = 3,
    GTK_IMAGE_NEW_ERR           = 4,
    GTK_STATUSBAR_NEW_ERR       = 5,
    GTK_ENTRY_NEW_ERR           = 6,
    GTK_WINDOW_NEW_ERR          = 7,
    GTK_STACK_NEW_ERR           = 8,
    GTK_DRAWING_AREA_NEW_ERR    = 9,

    GTK_INIT_MATRIX_ANIM_ERR    = 10,
    GTK_CREATE_FIRST_SCREEN     = 11,
    GTK_CREATE_SECOND_SCREEN    = 12,
    GTK_CREATE_MAIN_WINDOW_ERR  = 13,

};

#define CHECK_REFERENCE(ref, err_ref, err_code) \
    if (err_ref && ref == NULL) {               \
        *err_ref = (gtk_err) err_code;          \
        debug("error: %s", get_descr(err_ref)); \
        return;                                 \
    }                                           \

#define HANDLE_FUNC_ERROR(err_ref, err_code)    \
    if (err_ref && *err_ref != GTK_OK) {        \
        debug("error: %s", get_descr(err_ref));   \
        return;                                 \
    }                                           \

const char *get_descr(const enum gtk_err *err);

#endif // GTK_ERROR_PROC_H