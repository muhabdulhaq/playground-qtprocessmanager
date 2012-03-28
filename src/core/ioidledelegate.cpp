/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QFile>
#include <QDebug>
#include <QStringList>

#ifdef Q_OS_MAC
#include <mach/mach.h>
#include <mach/mach_host.h>
#include <mach/processor_info.h>
#endif

#include "ioidledelegate.h"

QT_BEGIN_NAMESPACE_PROCESSMANAGER

const int    kIdleTimerInterval = 1000;
const double kDefaultLoadThreshold = 0.4;

/*!
  \class IoIdleDelegate
  \brief The IoIdleDelegate class generates \l{idleCpuAvailable()} signals.

  The IoIdleDelegate determintes available CPU resources by watching
  the IO activity level on your system.  This delegate is appropriate
  when your system is primarily IO bound and not CPU bound.  When idle
  CPU resources are requested it checks the IO load level
  approximately once per second.  The calculated IO load level is
  measured by looking at the percentage of time spent in IO requests
  over the last time interval.  When the load level is below the
  threshold set by \l{loadThreshold}, the idleCpuAvailable() signal
  will be emitted.

  Under Linux, the \c{/proc/diskstats} file system object provides
  basic IO statistics use across all storage devices.  The
  IoIdleDelegate reads statistics for exactly one of these devices,
  from the \c{/sys/block/DEVICE/stat} file system entry.  To use the
  IoIdleDelegate under Linux, you must set the device property to be
  the name of the disk object.

  For example, on a standard Linux laptop, the hard disk usually shows
  up as \c{/dev/sda}, with appropriate file system partitions [on my
  laptop \c{/dev/sda1} is the main partition and \c{/dev/sda5} is the
  swap partition].  If I want to monitor io use on the main device, I
  could write:

  \code
    IoIdleDelegate *delegate = new IoIdleDelegate;
    delegate->setDevice("sda1");
  \endcode

  If I want to monitor disk use in general:

  \code
    delegate->setDevice("sda");
  \endcode

*/

/*!
  \property IoIdleDelegate::idleInterval
  \brief Time in milliseconds before a new idle CPU request will be fulfilled
 */

/*!
  \property IoIdleDelegate::device
  \brief Unix device name for the disk that you want to measure
 */

/*!
  \property IoIdleDelegate::loadThreshold
  \brief Load level we need to be under to generate idle CPU requests.

  This value is a double that ranges from 0.0 to 1.0.  A value greater
  than or equal to 1.0 guarantees that \l{idleCpuAvailable()} signals
  will always be emitted.  A value less than 0.0 blocks all \l{idleCpuAvailable()}
  signals.
 */


/*!
    Construct a IoIdleDelegate with an optional \a parent.
*/

IoIdleDelegate::IoIdleDelegate(QObject *parent)
    : IdleDelegate(parent)
    , m_load(1.0)
    , m_loadThreshold(kDefaultLoadThreshold)
    , m_last_msecs(0)
{
    connect(&m_timer, SIGNAL(timeout()), SLOT(timeout()));
    m_timer.setInterval(kIdleTimerInterval);
}

/*!
    Turn on or off idle requests based on \a state.
*/

void IoIdleDelegate::handleStateChange(bool state)
{
    if (state) {
        updateStats(true);
        m_elapsed.start();
        m_timer.start();
    }
    else {
        m_elapsed.invalidate();
        m_timer.stop();
    }
}

/*!
  \internal
 */

double IoIdleDelegate::updateStats(bool reset)
{
    int msecs = 0;
    m_load = 1.0;  // Always reset to max in case of error

#if defined(Q_OS_LINUX)
    QStringList stats;
    QFile file(QString::fromLatin1("/sys/block/%1/stat").arg(m_device));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString line = QString::fromLocal8Bit(file.readLine());
        stats = line.split(' ', QString::SkipEmptyParts);
    }
    if (stats.size() < 11)
        qFatal("Unable to read /sys/block stat file");

    msecs = stats.at(9).toInt();
#elif defined(Q_OS_MAC)
    // ### TODO
#endif
    if (!reset && m_last_msecs > 0 && m_elapsed.isValid()) {
        qint64 elapsed = m_elapsed.restart();
        if (elapsed > 0)
            m_load = (msecs - m_last_msecs) / (double) elapsed;
    }
    m_last_msecs = msecs;
    return m_load;
}

/*!
  \internal
 */

void IoIdleDelegate::timeout()
{
    if (updateStats(false) <= m_loadThreshold)
        emit idleCpuAvailable();
    emit loadUpdate(m_load);
}

/*!
  Return the current launch interval in milliseconds
 */

int IoIdleDelegate::idleInterval() const
{
    return m_timer.interval();
}

/*!
  Set the current idle interval to \a interval milliseconds
*/

void IoIdleDelegate::setIdleInterval(int interval)
{
    if (m_timer.interval() != interval) {
        m_timer.stop();
        m_timer.setInterval(interval);
        if (enabled() && requested())
            m_timer.start();
        emit idleIntervalChanged();
    }
}

/*!
  Return the current load threshold as a number from 0.0 to 1.0
 */

double IoIdleDelegate::loadThreshold() const
{
    return m_loadThreshold;
}

/*!
  Set the current load threshold to \a threshold milliseconds
*/

void IoIdleDelegate::setLoadThreshold(double threshold)
{
    if (m_loadThreshold != threshold) {
        m_loadThreshold = threshold;
        emit loadThresholdChanged();
    }
}

/*!
  Return the current IO device
 */

QString IoIdleDelegate::device() const
{
    return m_device;
}

/*!
  Set the current IO device
*/

void IoIdleDelegate::setDevice(const QString& device)
{
    if (m_device != device) {
        m_device = device;
        m_last_msecs = 0;
        emit deviceChanged();
    }
}

/*!
  \fn void IoIdleDelegate::idleIntervalChanged()
  This signal is emitted when the idleInterval is changed.
 */

/*!
  \fn void IoIdleDelegate::loadThresholdChanged()
  This signal is emitted when the loadThreshold is changed.
 */

/*!
  \fn void IoIdleDelegate::deviceChanged()
  This signal is emitted when the device is changed.
 */

/*!
  \fn void IoIdleDelegate::loadUpdate(double load)
  This signal is emitted when the load is read.  It mainly
  serves as a debugging signal.  The \a load value will
  range between 0.0 and 1.0
 */


#include "moc_ioidledelegate.cpp"

QT_END_NAMESPACE_PROCESSMANAGER
