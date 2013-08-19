#include "qhighdpiscaling_p.h"

bool qt_use_emulated_hidpi_mode_set = false;
bool qt_use_emulated_hidpi_mode = false;
qreal qt_high_dpi_scale_factor = 1.0;

bool qHighDpiIsScaled()
{
    // Get QT_HIGHDPI_SCALE_FACTOR, but only once.
    if (!qt_use_emulated_hidpi_mode_set) {
        QByteArray scaleFactorEnv = qgetenv("QT_HIGHDPI_SCALE_FACTOR");
        if (!scaleFactorEnv.isEmpty()) {
            bool ok;
            qt_high_dpi_scale_factor = scaleFactorEnv.toFloat(&ok);
            qt_use_emulated_hidpi_mode = ok;
        }
        qt_use_emulated_hidpi_mode_set = true;
    }

    return qt_use_emulated_hidpi_mode;
}

qreal qHighDpiScaleFactor()
{
    return qt_high_dpi_scale_factor;
}

