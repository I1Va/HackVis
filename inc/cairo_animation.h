#ifndef CAIRO_ANIMATION_H

#include <string.h>
#include <gtk/gtk.h>

static const int SHAKE_POSITIONS[] = {0, -4, 4, -4, 4, -3, 3, -2, 2, 0};
static const size_t SH_POS_SIZE = sizeof(SHAKE_POSITIONS) / sizeof(int);

const size_t NUM_COLUMNS = 50;
const int SHAKE_FIX_DELTA = 32;
const size_t ENG_LETTERS_CNT = 26;
const size_t MATRIX_FONT_SIZE = 14;
const char MATRIX_FONT[] = "Monospace";
const double MATRIX_BACK_RGB[3] = {0, 0, 0};
const double MATRIX_LET_RGB[3] = {0, 1, 0};
const size_t MATRIX_WIDTH = 400;
const size_t MATRIX_HEIGHT = 300;
const size_t MATRIX_RAIN_TIMEOUT = 100;
const int MATRIX_X_SCALE = 20;
const int MATRIX_MOD_SPEED = 5;
const int MATRIX_MIN_SPEDD = 2;

struct MatrixDrop {
    int x;
    int y;
    int speed;
};

struct matrix_anim_data_t {
    MatrixDrop drops[NUM_COLUMNS];
    GtkWidget *drawing_area;
};

void init_matrix_anim(matrix_anim_data_t *matrix_anim_data);
gboolean shake_window(gpointer window);

#endif // CAIRO_ANIMATION_H