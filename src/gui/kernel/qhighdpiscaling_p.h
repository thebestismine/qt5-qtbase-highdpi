
#include <QtCore/qglobal.h>
#include <QtCore/qmargins.h>

// Qt high-dpi scaling for operating systems that do not provide
// "retina"-type scaling support.
//
// These functons support creating a logical device-independent
// coordinate system which is related to the device pixel coordinate
// through a scaling factor. The scaling factor is set by the
// QT_HIGHDPI_SCALE_FACTOR environment variable.
//
// Most of the Qt API will then operate in the device-independent
// pixels. For example, setting the scale factor to 2.0 make Qt see
// half of the "device" window geometry.
//
// Integer scaling factors work best. Glitch free-graphics at non-integer
// scaling factors can not be guaranteed.

Q_GUI_EXPORT bool qHighDpiIsScaled();       // Returns true if a scaling factor is set
Q_GUI_EXPORT qreal qHighDpiScaleFactor();   // Returns the scaling factor if set, 1.0 otherwise.

// Coordinate system conversion functions:
// qHighDpiToDevicePixels               : from logical to physical (screen/backing) pixels
// qHighDpiToDeviceIndependentPixels    : from physical to logical pixels
inline QRect qHighDpiToDevicePixels(const QRect &pixelRect)
{
    const qreal scaleFactor = qHighDpiScaleFactor();
    return QRect(pixelRect.topLeft() / scaleFactor, pixelRect.size() / scaleFactor);
}

inline QRect qHighDpiToDeviceIndependentPixels(const QRect &pointRect)
{
    const qreal scaleFactor = qHighDpiScaleFactor();
    return QRect(pointRect.topLeft() * scaleFactor, pointRect.size() * scaleFactor);
}

inline QRectF qHighDpiToDevicePixels(const QRectF &pixelRect)
{
    const qreal scaleFactor = qHighDpiScaleFactor();
    return QRectF(pixelRect.topLeft() / scaleFactor, pixelRect.size() / scaleFactor);
}

inline QRectF qHighDpiToDeviceIndependentPixels(const QRectF &pointRect)
{
   const qreal scaleFactor = qHighDpiScaleFactor();
   return QRectF(pointRect.topLeft() * scaleFactor, pointRect.size() * scaleFactor);
}

inline QSize qHighDpiToDevicePixels(const QSize &pixelSize)
{
    return pixelSize / qHighDpiScaleFactor();
}

inline QSize qHighDpiToDeviceIndependentPixels(const QSize &pointSize)
{
    return pointSize * qHighDpiScaleFactor();
}

inline QSizeF qHighDpiToDevicePixels(const QSizeF &pixelSize)
{
    return pixelSize / qHighDpiScaleFactor();
}

inline QSizeF qHighDpiToDeviceIndependentPixels(const QSizeF &pointSize)
{
    return pointSize * qHighDpiScaleFactor();
}

inline QPoint qHighDpiToDevicePixels(const QPoint &pixelPoint)
{
    return pixelPoint / qHighDpiScaleFactor();
}

inline QPoint qHighDpiToDeviceIndependentPixels(const QPoint &pointPoint)
{
    return pointPoint * qHighDpiScaleFactor();
}

inline QPointF qHighDpiToDevicePixels(const QPointF &pixelPoint)
{
    return pixelPoint / qHighDpiScaleFactor();
}

inline QPointF qHighDpiToDeviceIndependentPixels(const QPointF &pointPoint)
{
    return pointPoint * qHighDpiScaleFactor();
}

inline QMargins qHighDpiToDevicePixels(const QMargins &pixelMargins)
{
    const qreal scaleFactor = qHighDpiScaleFactor();
    return QMargins(pixelMargins.left() / scaleFactor, pixelMargins.top() / scaleFactor,
                    pixelMargins.right() / scaleFactor, pixelMargins.bottom() / scaleFactor);
}

inline QMargins qHighDpiToDeviceIndependentPixels(const QMargins &pointMargins)
{
    const qreal scaleFactor = qHighDpiScaleFactor();
    return QMargins(pointMargins.left() * scaleFactor, pointMargins.top() * scaleFactor,
                    pointMargins.right() * scaleFactor, pointMargins.bottom() * scaleFactor);
}

inline QRegion qHighDpiToDevicePixels(const QRegion &pixelRegion)
{
    if (!qHighDpiIsScaled())
        return pixelRegion;

    QRegion pointRegion;
    foreach (const QRect &rect, pixelRegion.rects())
        pointRegion += qHighDpiToDevicePixels(rect);
    return pointRegion;
}

inline QRegion qHighDpiToDeviceIndependentPixels(const QRegion &pointRegion)
{
    if (!qHighDpiIsScaled())
        return pointRegion;

    QRegion pixelRegon;
    foreach (const QRect &rect, pointRegion.rects())
        pixelRegon += qHighDpiToDeviceIndependentPixels(rect);
    return pixelRegon;
}

// Any T that has operator/()
template <typename T>
T qHighDpiToDevicePixels(const T &pixelValue)
{
    if (!qHighDpiIsScaled())
        return pixelValue;

    return pixelValue / qHighDpiScaleFactor();

}

// Any T that has operator*()
template <typename T>
T qHighDpiToDeviceIndependentPixels(const T &pointValue)
{
    if (!qHighDpiIsScaled())
        return pointValue;

    return pointValue * qHighDpiScaleFactor();
}

// Any QVector<T> where T has operator/()
template <typename T>
QVector<T> qHighDpiToDevicePixels(const QVector<T> &pixelValues)
{
    if (!qHighDpiIsScaled())
        return pixelValues;

    QVector<T> pointValues;
    foreach (const T& pixelValue, pixelValues)
        pointValues.append(pixelValue / qHighDpiScaleFactor());
    return pointValues;
}

// Any QVector<T> where T has operator*()
template <typename T>
QVector<T> qHighDpiToDeviceIndependentPixels(const QVector<T> &pointValues)
{
    if (!qHighDpiIsScaled())
        return pointValues;

    QVector<T> pixelValues;
    foreach (const T& pointValue, pointValues)
        pixelValues.append(pointValue * qHighDpiScaleFactor());
    return pixelValues;
}


// Any QPair<T, U> where T and U has operator/()
template <typename T, typename U>
QPair<T, U> qHighDpiToDevicePixels(const QPair<T, U> &pixelPair)
{
    return qMakePair(qHighDpiToDevicePixels(pixelPair.first), qHighDpiToDevicePixels(pixelPair.second));
}

// Any QPair<T, U> where T and U has operator*()
template <typename T, typename U>
QPair<T, U> qHighDpiToDeviceIndependentPixels(const QPair<T, U> &pointPair)
{
    return qMakePair(qHighDpiToDeviceIndependentPixels(pointPair.first), qHighDpiToDeviceIndependentPixels(pointPair.second));
}

