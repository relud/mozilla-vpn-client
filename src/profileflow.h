/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PROFILEFLOW_H
#define PROFILEFLOW_H

#include "subscriptiondata.h"

#include <QObject>

class ProfileFlow final : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY_MOVE(ProfileFlow)

  Q_PROPERTY(State state READ state NOTIFY stateChanged);

 public:
  ProfileFlow();
  ~ProfileFlow();

  Q_INVOKABLE void start();
  Q_INVOKABLE void reset();

  enum State {
    StateInitial,
    StateLoading,
    StateAuthenticationNeeded,
    StateReady,
    StateError,
  };
  Q_ENUM(State);

  State state() const { return m_state; }

  void setForceReauthFlow(const bool forceReauthFlow);

 signals:
  void stateChanged(State state);

 private:
  void setState(State state);
  void subscriptionDetailsFetched(const QByteArray& subscriptionData);

 private:
  State m_state = StateInitial;

  bool m_forceReauthFlow = false;
};

#endif  // PROFILEFLOW_H
