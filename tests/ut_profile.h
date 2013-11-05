
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

#ifndef UT_PROFILE_H
#define UT_PROFILE_H

#include <QtTest/QtTest>
#include <QObject>
#include <QDBusInterface>
#include <QCoreApplication>

#include <Profile>
#include <profile_p.h>


class MocDBus : public DBusCaller
{
public:
    MocDBus();
    virtual ~MocDBus();

    void setReply(QDBusMessage msg);

    virtual QDBusMessage call(const QString & method,
                      const QVariant & arg1 = QVariant(),
                      const QVariant & arg2 = QVariant(),
                      const QVariant & arg3 = QVariant());
private:
    QDBusMessage *callReply;
};

class Ut_Profile : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testConstructionAndDestruction();
    void testActiveProfile();
    void testSetActiveProfile();
    void testProfileNames();
    void testVolumeLevel();
    void testTouchscreenVibrationLevel();
    void testIsVibrationEnabled();
    void testSetVolumeLevel();
    void testSetTouchscreenVibrationLevel();
    void testSetVibration();

    // signals to receive
    void testVolumeLevelChanged();
    void testTouchscreenVibrationLevelChanged();
    void testVibrationChanged();
    void testActiveProfileChanged();

private:
    Profile* m_subject;
    Profile* m_tmp;
    QCoreApplication *app;

    MocDBus *mocDbus;
};

#endif //UT_PROFILE_H
