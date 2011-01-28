
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

#include "profile.h"
#include "profile_p.h"

#include "profile_dbus.h"
#include "keys_nokia.h"

#include <QDBusInterface>
#include <QDBusMessage>
#include <QVariant>
#include <QList>
#include <QStringList>
#include <QDebug>
#include <QtDBus>
#include <QDBusArgument>

Q_DECLARE_METATYPE(QList<MyStructure>)


namespace ProfileConst
{
const QString boolOn = "On";
const QString boolOff = "Off";
}
// Marshall the MyStructure data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const MyStructure &mystruct)
{
    argument.beginStructure();
    argument << mystruct.key << mystruct.val << mystruct.type;
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, MyStructure &mystruct)
{
    argument.beginStructure();
    argument >> mystruct.key;
    argument >> mystruct.val;
    argument >> mystruct.type;
    argument.endStructure();
    return argument;
}

DBusCaller::DBusCaller()
{
}
DBusCaller::~DBusCaller()
{
}
QDBusMessage DBusCaller::call(const QString & method,
                              const QVariant & arg1,
                              const QVariant & arg2,
                              const QVariant & arg3)
{
    Q_UNUSED(method);
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    Q_UNUSED(arg3);
    return (new QDBusMessage())->createError(NULL, NULL);
}


NativeDBusCaller::NativeDBusCaller(const QString &service,
                                   const QString &path,
                                   const QString &interface)
{

    qDBusRegisterMetaType<MyStructure>();
    qDBusRegisterMetaType<QList<MyStructure > >();
    this->native = new QDBusInterface(service, path, interface, QDBusConnection::sessionBus());

    /* won't work for complex types..
    connect(this->native, SIGNAL(profile_changed(bool,bool,QString,QList<MyStructure>)),
            this, SIGNAL(handleProfileChanged(bool,bool,QString,QList<MyStructure>)));
    */
    bool succ = QDBusConnection::sessionBus().connect(service, path, interface,
                "profile_changed", QString("bbsa(sss)"), this,
                SIGNAL(profile_changed(bool, bool, QString, QList<MyStructure>)));
    if (!succ) {
        qDebug() << "libprofile-qt:: cannot connect to profiled.";
    }
}
NativeDBusCaller::~NativeDBusCaller()
{
    delete this->native;
    this->native = NULL;
}

QDBusMessage NativeDBusCaller::call(const QString & method,
                                    const QVariant & arg1,
                                    const QVariant & arg2,
                                    const QVariant & arg3)
{
    return native->call(method, arg1, arg2, arg3);
}


ProfilePrivate::ProfilePrivate()
{
    //qInstallMsgHandler(0);
    //qDebug() << Q_FUNC_INFO;
}

ProfilePrivate::~ProfilePrivate()
{
    //qDebug() << Q_FUNC_INFO;
    delete dbus;
    dbus = NULL;
}

Profile::Profile(ProfilePrivate* dd, QObject* parent) :
        QObject(parent),
        d_ptr(dd)
{
    //qDebug() << Q_FUNC_INFO;
    Q_D(Profile);
    if (d)
        d->q_ptr = this;
}

Profile::Profile(QObject* parent) :
        QObject(parent),
        d_ptr(new ProfilePrivate())

{
    //qDebug() << Q_FUNC_INFO;
    Q_D(Profile);
    d->q_ptr = this;
    d->dbus = new NativeDBusCaller(PROFILED_SERVICE, PROFILED_PATH, PROFILED_INTERFACE);

    d->activeProfile = activeProfile();
    QStringList names = profileNames();
    if (names.size() == 4) {
        for (int i = 0; i < names.size(); ++i) {
            d->names[i] = names.at(i);
            d->volumes[i] = volumeLevel(names.at(i));
            d->vibras[i] = isVibrationEnabled(names.at(i));
        }
    }

    connect(d->dbus, SIGNAL(profile_changed(bool, bool, QString, QList<MyStructure>)),
            this, SLOT(handleProfileChanged(bool, bool, QString, QList<MyStructure>)));
}

Profile::~Profile()
{
    //qDebug() << Q_FUNC_INFO;
    delete d_ptr;
}

QString Profile::activeProfile()
{
    //qDebug() << Q_FUNC_INFO;
    Q_D(Profile);
    QString name = "";
    QDBusMessage reply = d->dbus->call(PROFILED_GET_PROFILE);

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << Q_FUNC_INFO << "error reply:" << reply.errorName();
    } else if (reply.arguments().count() > 0) {
        name = reply.arguments().at(0).toString();
    }
    //qDebug() << Q_FUNC_INFO << name;
    return name;
}

bool Profile::setActiveProfile(QString name)
{
    qDebug() << Q_FUNC_INFO << name;
    Q_D(Profile);

    bool success = false;
    QDBusMessage reply = d->dbus->call(PROFILED_SET_PROFILE, QVariant(name));

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << Q_FUNC_INFO << "error reply:" << reply.errorName();
    } else if (reply.arguments().count() > 0) {
        success = reply.arguments().at(0).toBool();
    }
    return success;
}

QStringList Profile::profileNames()
{
    //qDebug() << Q_FUNC_INFO;
    Q_D(Profile);
    QStringList profileNames;
    QDBusMessage reply = d->dbus->call(PROFILED_GET_PROFILES);

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << Q_FUNC_INFO << "error reply:" << reply.errorName();
    } else if (reply.arguments().count() > 0) {
        QList<QVariant> qa = reply.arguments().at(0).toList();
        for (int i = 0; i < qa.count(); ++i) {
            profileNames.append(qa.at(i).toString());
        }
    }
    //qDebug() << Q_FUNC_INFO << profileNames;
    return profileNames;
}

int Profile::volumeLevel(QString profileName)
{
    //qDebug() << Q_FUNC_INFO << profileName;
    Q_D(Profile);
    // silent profile always has value 0
    if (ProfileName::silent == profileName) {
        return 0;
    }
    int volume = 30;
    QDBusMessage reply = d->dbus->call(PROFILED_GET_VALUE,
                                       QVariant(profileName),
                                       QVariant(PROFILEKEY_RINGING_ALERT_VOLUME));

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << Q_FUNC_INFO << "error reply:" << reply.errorName();
    } else if (reply.arguments().count() > 0) {
        volume = reply.arguments().at(0).toString().toInt();
    }
    return volume;
}

bool Profile::isVibrationEnabled(QString profileName)
{
    //qDebug() << Q_FUNC_INFO  << profileName;
    Q_D(Profile);
    bool vibra = true;
    QDBusMessage reply = d->dbus->call(PROFILED_GET_VALUE,
                                       QVariant(profileName),
                                       QVariant(PROFILEKEY_VIBRATING_ALERT_ENABLED));

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << Q_FUNC_INFO << "error reply:" << reply.errorName();
    } else if (reply.arguments().count() > 0) {
        vibra = (ProfileConst::boolOn == reply.arguments().at(0).toString() ? true : false);
    }
    return vibra;
}

bool Profile::setVolumeLevel(QString profileName, int level)
{
    //qDebug() << Q_FUNC_INFO << profileName << level;
    Q_D(Profile);

    // silent profile level can't be changed
    if (ProfileName::silent == profileName) {
        return true;
    }
    // limit the value in range 0-100
    if (level < 0)
        level = 0;
    if (level > 100)
        level = 100;

    bool success = false;
    QDBusMessage reply = d->dbus->call(PROFILED_SET_VALUE,
                                       QVariant(profileName),
                                       QVariant(PROFILEKEY_RINGING_ALERT_VOLUME),
                                       QVariant(QString::number(level)));

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << Q_FUNC_INFO << "error reply:" << reply.errorName();
    } else if (reply.arguments().count() > 0) {
        success = reply.arguments().at(0).toBool();
    }
    return success;
}

bool Profile::setVibration(QString profileName, bool enabled)
{
    //qDebug() << Q_FUNC_INFO << profileName << enabled;
    Q_D(Profile);
    bool success = false;
    QString val = (enabled ? ProfileConst::boolOn : ProfileConst::boolOff);
    QDBusMessage reply = d->dbus->call(PROFILED_SET_VALUE,
                                       QVariant(profileName),
                                       QVariant(PROFILEKEY_VIBRATING_ALERT_ENABLED),
                                       QVariant(val));

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << Q_FUNC_INFO << "error reply:" << reply.errorName();
    } else if (reply.arguments().count() > 0) {
        success = reply.arguments().at(0).toBool();
    }
    return success;
}

void Profile::handleProfileChanged(bool changed, bool active, QString profile,
                                   QList<MyStructure> keyValType)
{
    /*
    QStringList keyval;
    for(int i = 0; i < keyValType.count(); ++i)
    {
        MyStructure s = keyValType.at(i);        
        keyval << QString(s.key+", "+s.val+", "+s.type+"\n");
    }
    qDebug() << Q_FUNC_INFO << "changed:" << changed << "active:" << profile << "keyValType:" << keyval;
    */
    Q_D(Profile);

    if (changed && active) {
        if (d->activeProfile != profile) {
            emit activeProfileChanged(profile);
            d->activeProfile = profile;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (d->names[i] == profile) {
            // check for changes
            for (int j = 0; j < keyValType.size(); ++j) {
                MyStructure s = keyValType.at(j);
                if (s.key == PROFILEKEY_VIBRATING_ALERT_ENABLED) {
                    bool vibra = s.val == ProfileConst::boolOn ? true : false;
                    if (vibra != d->vibras[i]) {
                        emit vibrationChanged(profile, vibra);
                        d->vibras[i] = vibra;
                    }
                } else if (s.key == PROFILEKEY_RINGING_ALERT_VOLUME) {
                    int volume = s.val.toInt();
                    if (volume != d->volumes[i]) {
                        emit volumeLevelChanged(profile, volume);
                        d->volumes[i] = volume;
                    }
                }
            }
        }
    }

}


