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

#include "qkeymatchdelegate.h"
#include "qprocessinfo.h"

QT_BEGIN_NAMESPACE_PROCESSMANAGER

/*!
  \class QKeyMatchDelegate
  \brief The QKeyMatchDelegate class matches based on a key-value pair
  \inmodule QtProcessManager

  The QKeyMatchDelegate class matches based on key-value pair.  It only
  matches QProcessInfo records which contain "key".  If a "value" attribute
  is also set, then the QKeyMatchDelegate will only match a QProcessInfo
  record if the value of the "key" record matches the value.
*/

/*!
  \property QKeyMatchDelegate::key
  \brief The key value required in the QProcessInfo object for a match.
 */

/*!
  \property QKeyMatchDelegate::value
  \brief If set, this value must match the record in the QProcessInfo object.
 */

/*!
    Construct a QKeyMatchDelegate with an optional \a parent.
*/

QKeyMatchDelegate::QKeyMatchDelegate(QObject *parent)
    : QMatchDelegate(parent)
{
}

/*!
  Return the current key
*/

QString QKeyMatchDelegate::key() const
{
    return m_key;
}

/*!
  Set a new \a key.
 */

void QKeyMatchDelegate::setKey(const QString& key)
{
    m_key = key;
    emit keyChanged();
}

/*!
  Return the current value
*/

QVariant QKeyMatchDelegate::value() const
{
    return m_value;
}

/*!
  Set a new \a value.
 */

void QKeyMatchDelegate::setValue(const QVariant& value)
{
    m_value = value;
    emit valueChanged();
}

/*!
    \fn QKeyMatchDelegate::matches(const QProcessInfo& info)
    \brief Check \a info for key and value attributes
*/

bool QKeyMatchDelegate::matches(const QProcessInfo& info)
{
    return (info.contains(m_key) && (m_value.isNull() || info.value(m_key) == m_value));
}

/*!
  \fn void QKeyMatchDelegate::keyChanged()
  Signal emitted when the key on this delegate has been changed.
*/

/*!
  \fn void QKeyMatchDelegate::valueChanged()
  Signal emitted when the value on this delegate has been changed.
*/

#include "moc_qkeymatchdelegate.cpp"

QT_END_NAMESPACE_PROCESSMANAGER
