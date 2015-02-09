
#define PYVI_CALIBRATION 0
#define PYVI_1 1
#define PYVI_2 2
#define PYVI_3 3
#define PYVI_4 4
#define PYVI_5 5
#define PYVI_6 6
#define PYVI_7 7

#if PYVI_BOARD == PYVI_CALIBRATION
#   define FIRMWARE_FOR_CALIBRATION
#   include "board_calibration_data/pivy_default.h"
#elif PYVI_BOARD == PYVI_1
#   include "board_calibration_data/pivy_1.h"
#elif PYVI_BOARD == PYVI_2
#   include "board_calibration_data/pivy_2.h"
#elif PYVI_BOARD == PYVI_3
#   include "board_calibration_data/pivy_3.h"
#elif PYVI_BOARD == PYVI_4
#   include "board_calibration_data/pivy_4.h"
#elif PYVI_BOARD == PYVI_5
#   include "board_calibration_data/pivy_5.h"
#elif PYVI_BOARD == PYVI_6
#   include "board_calibration_data/pivy_6.h"
#elif PYVI_BOARD == PYVI_7
#   include "board_calibration_data/pivy_7.h"
#else
#   error Missing PYVV_BOARD calibration_data
#endif
