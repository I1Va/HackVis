#include <stdlib.h>
#include "HackVis/gtk_error_proc.h"

const char *hackvis_get_descr(const enum gtk_err *err) {
    #define DESCR_(err) \
    case err:           \
        return #err;    \

    switch (*err) {
        DESCR_(GTK_OK                  )
        DESCR_(GTK_OVERLAY_NEW_ERR     )
        DESCR_(GTK_BOX_NEW_ERR         )
        DESCR_(GTK_BUTTON_NEW_ERR      )
        DESCR_(GTK_IMAGE_NEW_ERR       )
        DESCR_(GTK_STATUSBAR_NEW_ERR   )
        DESCR_(GTK_ENTRY_NEW_ERR       )
        DESCR_(GTK_WINDOW_NEW_ERR      )
        DESCR_(GTK_STACK_NEW_ERR       )
        DESCR_(GTK_DRAWING_AREA_NEW_ERR)

        DESCR_(GTK_INIT_MATRIX_ANIM_ERR)
        DESCR_(GTK_CREATE_START_DIALOG_BOX_ERR )
        DESCR_(GTK_CREATE_HACKING_BOX)
        DESCR_(GTK_CREATE_MAIN_WINDOW_ERR)
        default:
            return "!UNKNOWN ERROR!";
            break;
    }
}

#undef DESCR_