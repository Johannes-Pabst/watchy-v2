#include "display/alignment.h"
#include <Arduino.h>

void splitAlignment(u_int8_t td, uint8_t &x_dir, uint8_t &y_dir)
{
    switch (td)
    {
    case TD_TOP_LEFT:
        x_dir = TD1_START;
        y_dir = TD1_START;
        break;
    case TD_TOP_CENTER:
        x_dir = TD1_CENTER;
        y_dir = TD1_START;
        break;
    case TD_TOP_RIGHT:
        x_dir = TD1_END;
        y_dir = TD1_START;
        break;
    case TD_CENTER_LEFT:
        x_dir = TD1_START;
        y_dir = TD1_CENTER;
        break;
    case TD_CENTER_CENTER:
        x_dir = TD1_CENTER;
        y_dir = TD1_CENTER;
        break;
    case TD_CENTER_RIGHT:
        x_dir = TD1_END;
        y_dir = TD1_CENTER;
        break;
    case TD_BOTTOM_LEFT:
        x_dir = TD1_START;
        y_dir = TD1_END;
        break;
    case TD_BOTTOM_CENTER:
        x_dir = TD1_CENTER;
        y_dir = TD1_END;
        break;
    case TD_BOTTOM_RIGHT:
        x_dir = TD1_END;
        y_dir = TD1_END;
        break;
    }
}