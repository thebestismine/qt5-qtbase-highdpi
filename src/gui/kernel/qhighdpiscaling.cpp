/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qhighdpiscaling_p.h"
#include "qwindow_p.h" // for QWINDOWSIZE_MAX
#include "qguiapplication.h"
#include "qscreen.h"
#include "private/qscreen_p.h"

#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcScaling, "qt.scaling");


// Get a qreal from the given environemnt variable. Returns
// true on success: the env. variable was set and it contained
// someting convertible to qreal.
static bool getEnvReal(qreal *factor, const char *envVar)
{
    if (qEnvironmentVariableIsSet(envVar)) {
        bool ok;
        const qreal f = qgetenv(envVar).toDouble(&ok);
        if (ok && f > 0) {
            qCDebug(lcScaling) << "Apply" << envVar << f;
            *factor = f;
            return true;
        }
    }
    return false;
}

static inline qreal getInitialScaleFactor(bool *scaleDpi, bool *isFactor)
{
    qreal factor = 1;
    if (getEnvReal(&factor, "QT_SCALE_FACTOR")) {
        *scaleDpi = false;
        *isFactor = true;
    } else if (getEnvReal(&factor, "QT_SCALE_FACTOR_DPI")) {
        *scaleDpi = true;
        *isFactor = true;
    } else if (getEnvReal(&factor, "QT_SCALE")) {
        *scaleDpi = false;
        *isFactor = false;
    } else if (getEnvReal(&factor, "QT_SCALE_DPI")) {
        *scaleDpi = true;
        *isFactor = false;
    }
    return factor;
}

/*!
    \class QHighDpiScaling
    \since 5.4
    \internal
    \preliminary
    \ingroup qpa

    \brief Collection of utility functions for UI scaling.
*/

bool  QHighDpiScaling::m_scaleDpi = false;
bool  QHighDpiScaling::m_isFactor = true;
qreal QHighDpiScaling::m_factor = getInitialScaleFactor(&m_scaleDpi, &m_isFactor);
bool QHighDpiScaling::m_active = !qFuzzyCompare(QHighDpiScaling::m_factor, qreal(1));
bool QHighDpiScaling::m_perWindowActive = false;

void QHighDpiScaling::setFactor(qreal factor)
{
    if (qFuzzyCompare(factor, QHighDpiScaling::m_factor))
        return;
    if (!QGuiApplication::allWindows().isEmpty()) {
        qWarning() << Q_FUNC_INFO << "QHighDpiScaling::setFactor: Should only be called when no windows exist.";
    }

    QHighDpiScaling::m_active = !qFuzzyCompare(factor, qreal(1));
    QHighDpiScaling::m_factor = QHighDpiScaling::m_active ? factor : qreal(1);
    QHighDpiScaling::m_isFactor = true; // setFactor sets a factor.
    Q_FOREACH (QScreen *screen, QGuiApplication::screens())
         screen->d_func()->updateHighDpi();
}

static const char *scaleFactorProperty = "_q_scaleFactor";

void QHighDpiScaling::setWindowFactor(QWindow *window, qreal factor)
{
    m_perWindowActive = true;
    window->setProperty(scaleFactorProperty, QVariant(factor));
}

qreal QHighDpiScaling::factor(const QWindow *window)
{
    if (window == 0)
        return m_factor;

    if (!m_perWindowActive) {
        if (m_isFactor)
            return m_factor;
        else
            return m_factor / (window->handle()? window->handle()->devicePixelRatio() : qreal(1.0));
    }

    QVariant windowFactor = window->property(scaleFactorProperty);
    return m_factor * (windowFactor.isValid() ? windowFactor.toReal() : 1);
}

bool QHighDpiScaling::scaleDpi()
{
    return m_scaleDpi;
}

Q_GUI_EXPORT QSize qHighDpiToDevicePixelsConstrained(const QSize &size, const QWindow *window)
{
    const int width = size.width();
    const int height = size.height();
    return QSize(width > 0 && width < QWINDOWSIZE_MAX ?
                 qHighDpiToDevicePixels(width, window) : width,
                 height > 0 && height < QWINDOWSIZE_MAX ?
                 qHighDpiToDevicePixels(height, window) : height);
}

QT_END_NAMESPACE
