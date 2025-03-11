
#ifndef GTK_ERROR_PROC_H
#define GTK_ERROR_PROC_H

#include "general.h"

enum gtk_err
{
    GTK_OK                          ,
    GTK_OVERLAY_NEW_ERR             ,
    GTK_BOX_NEW_ERR                 ,
    GTK_BUTTON_NEW_ERR              ,
    GTK_IMAGE_NEW_ERR               ,
    GTK_STATUSBAR_NEW_ERR           ,
    GTK_ENTRY_NEW_ERR               ,
    GTK_WINDOW_NEW_ERR              ,
    GTK_STACK_NEW_ERR               ,
    GTK_DRAWING_AREA_NEW_ERR        ,

    GTK_INIT_MATRIX_ANIM_ERR        ,
    GTK_CREATE_START_DIALOG_BOX_ERR ,
    GTK_CREATE_HACKING_BOX          ,
    GTK_CREATE_MAIN_WINDOW_ERR      ,

};

#define HACKVIS_CHECK_REFERENCE(ref, err_ref, err_code) \
    if (err_ref && ref == NULL) {                       \
        *err_ref = (gtk_err) err_code;                  \
        debug("error: %s", hackvis_get_descr(err_ref));         \
        return;                                         \
    }                                                   \

#define HACKVIS_HANDLE_FUNC_ERROR(err_ref, err_code)    \
    if (err_ref && *err_ref != GTK_OK) {                \
        debug("error: %s", hackvis_get_descr(err_ref));         \
        return;                                         \
    }                                                   \

const char *hackvis_get_descr(const enum gtk_err *err);

#endif // GTK_ERROR_PROC_H