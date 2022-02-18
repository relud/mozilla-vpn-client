/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "wasmauthenticationlistener.h"
#include "leakdetector.h"
#include "logger.h"
#include "settingsholder.h"
#include "tutorial.h"

#include <emscripten/val.h>
#include <QTimer>
#include <QUrlQuery>

namespace {

Logger logger(LOG_MAIN, "WasmAuthenticationListener");

}  // anonymous namespace

WasmAuthenticationListener::WasmAuthenticationListener(QObject* parent)
    : AuthenticationListener(parent) {
  MVPN_COUNT_CTOR(WasmAuthenticationListener);
}

WasmAuthenticationListener::~WasmAuthenticationListener() {
  MVPN_COUNT_DTOR(WasmAuthenticationListener);
}

void WasmAuthenticationListener::start(Task* task, const QString& codeChallenge,
                                       const QString& codeChallengeMethod,
                                       const QString& emailAddress) {
  logger.debug() << "WasmAuthenticationListener initialize";

  Q_UNUSED(task);
  Q_UNUSED(codeChallenge);
  Q_UNUSED(codeChallengeMethod);
  Q_UNUSED(emailAddress);

  emscripten::val location = emscripten::val::global("location");
  QString queryString(location["search"].as<std::string>().c_str());

  QUrlQuery query(queryString.remove(0, 1));

  QString tutorialPath;
  QList<QPair<QString, QString>> params = query.queryItems();
  for (const QPair<QString, QString>& param : params) {
    if (param.first == "tutorial") {
      tutorialPath = param.second;
      break;
    }
  }

  logger.debug() << "Tutorial" << tutorialPath;

  if (tutorialPath.isEmpty()) {
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,
            [this]() { emit completed("WASM"); });
    timer->start(2000);
    return;
  }

  SettingsHolder* settingsHolder = SettingsHolder::instance();
  Q_ASSERT(settingsHolder);

  settingsHolder->setPostAuthenticationShown(true);
  settingsHolder->setTelemetryPolicyShown(true);

  Tutorial::instance()->play(tutorialPath);

  emit completed("WASM");
}
