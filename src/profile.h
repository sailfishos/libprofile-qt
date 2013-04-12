
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

#ifndef PROFILES_H
#define PROFILES_H

#include <QObject>
#include <QStringList>
#include <QList>

#include <QtCore/QtGlobal>
#include <QtDBus>


#if defined(PROFILE_QT)
#    define P_EXPORT Q_DECL_EXPORT
#else
#    if defined (Q_OS_WIN)
#        define P_EXPORT Q_DECL_IMPORT
#    else
#        define P_EXPORT Q_DECL_EXPORT
#    endif
#endif

struct MyStructure {
    QString key, val, type;
};
QDBusArgument &operator<<(QDBusArgument &a, const MyStructure &mystruct);
const QDBusArgument &operator>>(const QDBusArgument &a, MyStructure &mystruct);
Q_DECLARE_METATYPE(MyStructure)


class ProfilePrivate;

/*!
 * Profile namespace.
 * Name constants for profiles.
 */
namespace ProfileName
{
const QString ringing = "general";  /*!< Profile name 'ringing' */
const QString silent = "silent";    /*!< Profile name 'silent' */
const QString beep = "meeting";        /*!< Profile name 'beep' */
const QString loud = "outdoors";        /*!< Profile name 'loud' */
}

/*!
 * An interface class to get and set phone default profiles and change the volume and vibration
 * settings for different events and environments.
 */
class P_EXPORT Profile : public QObject
{
    Q_OBJECT

public:
    /*!
     * A constructor.
     * Constructs a profile with \e parent.
     * \param parent Parent of the class.
     */
    explicit Profile(QObject* parent = 0);
    /*!
     * A destructor.
     */
    virtual ~Profile();

public Q_SLOTS:

    /*!
     * Get the name of active profile.
     *
     * \return profile name. The profile name constants are defined in \b TBD \c .h
     **/
    QString activeProfile();

    /*!
     * Set active profile.
     *
     * \param name of the profile to set active. The profile name constants are defined in \b TBD \c .h
     * \return status. If operation succeeds, returns \e true, \e false otherways.
     **/
    bool setActiveProfile(QString name);

    /**
     * Get available profile names.
     *
     * \return supported profile names in a list
     **/
    QStringList profileNames();

    /*!
     * Returns volume level.
     * Volume level range is 0 - 100 where 0 means no ringing.
     * Silent profile has always volume level 0.
     *
     * \param profileName Name of the profile interested of.
     * \return level of the volume.
     * \sa setVolumeLevel(QString profileName, int level)
     **/
    int volumeLevel(QString profileName);

    /*!
     * Check if vibration is enabled.
     *
     * \param profileName Name of the profile interested of.
     * \return \e True if vibration is enabled and \e false if vibration is disabled.
     * \sa setVibration(QString profileName, bool enabled)
     **/
    bool isVibrationEnabled(QString profileName);

    /*!
     * Set volume level.
     * Volume level range is 0 - 100 where 0 means no ringing.
     * \e Level is set to 0 or 100 if value is outside of the range.
     * Silent profile has always volume level 0 and it can't change.
     *
     * \param profileName Name of the profile to modify
     * \param level New volume level to set for the profile. \sa getVolumeLevel(QString profileName)
     * \return status. If operation succeeds, returns \e true, \e false otherways.
     **/
    bool setVolumeLevel(QString profileName, int level);

    /*!
     * Set vibration.
     *
     * \param profileName Name of the profile to modify
     * \param enabled \e True to set the vibration enabled, \e false to set vibration
     *      disabled. \sa isVibrationEnabled(QString profileName)
     * \return status. If operation succeeds, returns \e true, \e false otherways.
     **/
    bool setVibration(QString profileName, bool enabled);

signals:

    /*!
     * Signal emitted after changes to profile data
     * Volume level range is 0 - 100 where 0 means no ringing.
     * Silent profile has always volume level 0 and it can't change.
     *
     * \param profileName Name of the changed profile
     * \param level \e new volume level
     * \sa setVolumeLevel(QString profileName, int level),
     **/
    void volumeLevelChanged(QString profileName, int level);

    /*!
     * Signal emitted after changes to profile data
     *
     * \param profileName Name of the changed profile
     * \param enabled \e True if the vibration is enabled, \e false if the vibration
     *      is disabled.
     * \sa isVibrationEnabled(QString profileName), setVibration(QString profileName, bool enabled)
     **/
    void vibrationChanged(QString profileName, bool enabled);

    /*!
     * Signal emitted when active profile is changed to new one.
     * \e {
     * Note: connecting object needs to maintain itself what was the
     * old active profile. API delivers only the name of the new profile.
     * }
     * \param newActiveProfileName name of the new active profile
     **/
    void activeProfileChanged(QString newActiveProfileName);

private slots:
    void handleProfileChanged(bool changed, bool active, QString profile, QList<MyStructure> keyValType);

private:
    ProfilePrivate* const d_ptr;
    Profile(ProfilePrivate* dd, QObject* parent = 0);

private:
    Q_DECLARE_PRIVATE(Profile)
    Q_DISABLE_COPY(Profile)

#ifdef UNIT_TEST
    friend class Ut_Profile;
#endif
};

#endif // PROFILES_H
