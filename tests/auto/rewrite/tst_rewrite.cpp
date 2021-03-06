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

#include <QtTest>

#include "qgdbrewritedelegate.h"
#include "qprocessinfo.h"

QT_USE_NAMESPACE_PROCESSMANAGER

/******************************************************************************/

class TestRewrite : public QObject
{
    Q_OBJECT
public:
    TestRewrite(QObject *parent=0);

private Q_SLOTS:
    void rewriteGdb();
};

TestRewrite::TestRewrite(QObject *parent)
  : QObject(parent)
{
}

void TestRewrite::rewriteGdb()
{
    QProcessInfo info;
    info.setProgram("/usr/bin/abc");
    info.setArguments(QStringList() << "a" << "b");

    QGdbRewriteDelegate delegate;
    delegate.rewrite(info);

    QCOMPARE(info.program(), QLatin1String("gdb"));
    QCOMPARE(info.arguments().size(), 4);
    QCOMPARE(info.arguments().at(0), QLatin1String("--"));
    QCOMPARE(info.arguments().at(1), QLatin1String("/usr/bin/abc"));
    QCOMPARE(info.arguments().at(2), QLatin1String("a"));
    QCOMPARE(info.arguments().at(3), QLatin1String("b"));
}

QTEST_MAIN(TestRewrite)
#include "tst_rewrite.moc"
