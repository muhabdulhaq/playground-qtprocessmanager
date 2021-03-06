/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtAddOn.JsonStream module of the Qt.
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

#ifndef UNIX_PROCESS_BACKEND_H
#define UNIX_PROCESS_BACKEND_H

#include "qprocessbackend.h"
#include <QTimer>

#include "qprocessmanager-global.h"

QT_BEGIN_NAMESPACE_PROCESSMANAGER

class Q_ADDON_PROCESSMANAGER_EXPORT QUnixProcessBackend : public QProcessBackend
{
    Q_OBJECT

public:
    QUnixProcessBackend(const QProcessInfo& info, QObject *parent=0);
    virtual ~QUnixProcessBackend();

    virtual Q_PID  pid() const;

    virtual qint32 actualPriority() const;
    virtual void   setDesiredPriority(qint32);

#if defined(Q_OS_LINUX)
    virtual qint32 actualOomAdjustment() const;
    virtual void   setDesiredOomAdjustment(qint32);
#endif

    virtual QProcess::ProcessState state() const;
    virtual void stop(int timeout = 500);

    virtual qint64 write(const char *data, qint64 maxSize);

    virtual QString errorString() const;

protected:
    bool createProcess();
    void startProcess();

    virtual void handleProcessStarted();
    virtual void handleProcessError(QProcess::ProcessError error);
    virtual void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    virtual void handleProcessStateChanged(QProcess::ProcessState state);

private slots:
    void unixProcessStarted();
    void unixProcessError(QProcess::ProcessError error);
    void unixProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void unixProcessStateChanged(QProcess::ProcessState state);

    void killTimeout();
    void readyReadStandardOutput();
    void readyReadStandardError();

protected:
    QProcess           *m_process;
    QTimer              m_killTimer;
};

QT_END_NAMESPACE_PROCESSMANAGER

#endif // UNIX_PROCESS_BACKEND_H
