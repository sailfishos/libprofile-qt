
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

#include "ut_profile.h"

#include "profile_dbus.h"
#include "keys_nokia.h"

#include <QDBusInterface>
#include <QDBusMessage>
#include <QStringList>
#include <QVariant>
#include <QDebug>

#include<QList>

#include <QDBusConnection>

#include <MApplication>
#include <QSignalSpy>


using namespace ProfileName;

MocDBus::MocDBus()
{
    qDBusRegisterMetaType<MyStructure>();
}

MocDBus::~MocDBus()
{
}

void MocDBus::setReply(QDBusMessage msg)
{
    this->callReply = &msg;
}

QDBusMessage MocDBus::call(const QString & method,
                      const QVariant & arg1,
                      const QVariant & arg2,
                      const QVariant & arg3)
{
    Q_UNUSED(method);
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    Q_UNUSED(arg3);

    QDBusMessage *ret = callReply;
    this->callReply = NULL;
    return *ret;
}


////////////////////
void Ut_Profile::init()
{
}

void Ut_Profile::cleanup()
{
}

void Ut_Profile::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./ut_profile";
    app = new MApplication(argc, &app_name);

    ProfilePrivate *priv = new ProfilePrivate();
    m_subject = new Profile(priv);
    mocDbus = new MocDBus();
    m_subject->d_ptr->dbus = mocDbus;

    m_subject->d_ptr->activeProfile = ProfileName::ringing;
    m_subject->d_ptr->names[0] = ProfileName::ringing;
    m_subject->d_ptr->names[1] = ProfileName::loud;
    m_subject->d_ptr->names[2] = ProfileName::beep;
    m_subject->d_ptr->names[3] = ProfileName::silent;
    m_subject->d_ptr->vibras[0] = true;
    m_subject->d_ptr->vibras[1] = true;
    m_subject->d_ptr->vibras[2] = false;
    m_subject->d_ptr->vibras[3] = true;
    m_subject->d_ptr->volumes[0] = 10;
    m_subject->d_ptr->volumes[1] = 20;
    m_subject->d_ptr->volumes[2] = 30;
    m_subject->d_ptr->volumes[3] = 0;
}

void Ut_Profile::cleanupTestCase()
{
    delete app;
    app = NULL;
    delete m_subject;
    m_subject = NULL;
}

void Ut_Profile::testConstructionAndDestruction()
{
    /*
    m_tmp = new Profile();
    QVERIFY(m_tmp != NULL);
    delete m_tmp;
    m_tmp = NULL;
    */
}

void Ut_Profile::testProfileNames()
{
    //QDBusMessage msg = QDBusMessage::createMethodCall("a", "/b/b", "a", "b");
    QDBusMessage msg;
    QList<QVariant> names;
    names << ProfileName::ringing << ProfileName::silent
          << ProfileName::loud << ProfileName::beep;
    QList<QVariant> args;
    args << names;
    msg << args;
    mocDbus->setReply(msg);

    QStringList list = m_subject->profileNames();

    QVERIFY(list.contains(ProfileName::ringing));
    QVERIFY(list.contains(ProfileName::silent));
    QVERIFY(list.contains(ProfileName::loud));
    QVERIFY(list.contains(ProfileName::beep));
}

void Ut_Profile::testActiveProfile()
{
    QDBusMessage msg;
    msg << QVariant(ProfileName::loud);
    mocDbus->setReply(msg);

    QVERIFY(ProfileName::loud == m_subject->activeProfile());
}

void Ut_Profile::testSetActiveProfile()
{
    QDBusMessage msg;
    msg << QVariant(true);
    mocDbus->setReply(msg);

    QVERIFY(m_subject->setActiveProfile("anything"));
}

void Ut_Profile::testVolumeLevel()
{
    QDBusMessage msg;
    msg << QVariant(42);
    mocDbus->setReply(msg);

    QVERIFY(42 == m_subject->volumeLevel(ProfileName::ringing));
    QVERIFY(0 == m_subject->volumeLevel(ProfileName::silent));
}

void Ut_Profile::testIsVibrationEnabled()
{
    QDBusMessage msg;
    msg << QVariant("On");
    mocDbus->setReply(msg);

    QVERIFY(m_subject->isVibrationEnabled("anything"));
}

void Ut_Profile::testSetVolumeLevel()
{
    QDBusMessage msg;
    msg << QVariant(123);
    mocDbus->setReply(msg);

    QVERIFY(m_subject->setVolumeLevel("anything", 123));
}

void Ut_Profile::testSetVibration()
{
    QDBusMessage msg;
    msg << QVariant(true);
    mocDbus->setReply(msg);

    QVERIFY(m_subject->setVolumeLevel("anything", true));
}

void Ut_Profile::testVolumeLevelChanged()
{
    /*
    m_subject->d_ptr->activeProfile = ProfileName::ringing;
    m_subject->d_ptr->names[0] = ProfileName::ringing;
    m_subject->d_ptr->vibras[0] = true;
    m_subject->d_ptr->volumes[0] = 10;
    */

    QList<MyStructure> arr;
    MyStructure foo = { PROFILEKEY_RINGING_ALERT_VOLUME, "23", "type" };
    arr << foo;
    QSignalSpy spy(m_subject, SIGNAL(volumeLevelChanged(QString,int)));

    m_subject->handleProfileChanged(false, false, ProfileName::ringing, arr);

    QVERIFY(spy.count() == 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), ProfileName::ringing);
    QCOMPARE(arguments.at(1).toInt(), 23);

    // same again
    m_subject->handleProfileChanged(false, false, ProfileName::ringing, arr);
    QVERIFY(spy.count() == 0);
}
void Ut_Profile::testVibrationChanged()
{
    /*
    m_subject->d_ptr->activeProfile = ProfileName::ringing;
    m_subject->d_ptr->names[0] = ProfileName::ringing;
    m_subject->d_ptr->vibras[0] = true;
    m_subject->d_ptr->volumes[0] = 10;
    */

    QList<MyStructure> arr;
    MyStructure foo = { PROFILEKEY_VIBRATING_ALERT_ENABLED, "Off", "type" };
    arr << foo;
    QSignalSpy spy(m_subject, SIGNAL(vibrationChanged(QString,bool)));

    m_subject->handleProfileChanged(false, false, ProfileName::ringing, arr);

    QVERIFY(spy.count() == 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), ProfileName::ringing);
    QCOMPARE(arguments.at(1).toBool(), false);

    // same again
    m_subject->handleProfileChanged(false, false, ProfileName::ringing, arr);
    QVERIFY(spy.count() == 0);
}
void Ut_Profile::testActiveProfileChanged()
{
    QList<MyStructure> arr;
    MyStructure foo = { PROFILEKEY_RINGING_ALERT_VOLUME, "30", "int" };
    arr << foo;
    QSignalSpy activeSig(m_subject, SIGNAL(activeProfileChanged(QString)));
    m_subject->handleProfileChanged(true, true, ProfileName::loud, arr);
    QVERIFY(activeSig.count() == 1);

    activeSig.clear();
    m_subject->handleProfileChanged(true, true, ProfileName::loud, arr);
    QVERIFY(activeSig.count() == 0);

}

QTEST_APPLESS_MAIN(Ut_Profile)
