
/******************************************************************************
** This file is part of profile-qt
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
**
** Contact: Sakari Poussa <sakari.poussa@nokia.com>
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
** Redistributions of source code must retain the above copyright notice,
** this list of conditions and the following disclaimer. Redistributions in
** binary form must reproduce the above copyright notice, this list of
** conditions and the following disclaimer in the documentation  and/or
** other materials provided with the distribution.
**
** Neither the name of Nokia Corporation nor the names of its contributors
** may be used to endorse or promote products derived from this software 
** without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
** THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
** CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
** OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
** OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
** ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#ifndef PROFILE_P_H
#define PROFILE_P_H

#include "profile.h"

#include <QtDBus>
#include <QDBusMessage>
#include <QDBusArgument>
#include <QList>

#define PROFILE_MAX_PROFILES	10

class QDBusInterface;

class DBusCaller : public QObject
{
    Q_OBJECT
public:
    DBusCaller();
    virtual ~DBusCaller();

    virtual QDBusMessage call(const QString & method,
                              const QVariant & arg1 = QVariant(),
                              const QVariant & arg2 = QVariant(),
                              const QVariant & arg3 = QVariant());
signals:
    void profile_changed(bool changed, bool active, QString profile, QList<MyStructure> values);
};

class NativeDBusCaller : public DBusCaller
{
    Q_OBJECT
public:
    NativeDBusCaller(const QString &service,
                     const QString &path,
                     const QString &interface);
    ~NativeDBusCaller();
    virtual QDBusMessage call(const QString & method,
                              const QVariant & arg1 = QVariant(),
                              const QVariant & arg2 = QVariant(),
                              const QVariant & arg3 = QVariant());

private:
    QDBusInterface *native;
};

class ProfilePrivate
{
    Q_DECLARE_PUBLIC(Profile)

public:
    ProfilePrivate();
    virtual ~ProfilePrivate();

    DBusCaller *dbus;

protected:
    Profile* q_ptr;

    QString activeProfile;
    QStringList names;
    int volumes[PROFILE_MAX_PROFILES];
    bool vibras[PROFILE_MAX_PROFILES];
    int vibraLevels[PROFILE_MAX_PROFILES];

#ifdef UNIT_TEST
    friend class Ut_Profile;
#endif
};

#endif // PROFILE_P_H
