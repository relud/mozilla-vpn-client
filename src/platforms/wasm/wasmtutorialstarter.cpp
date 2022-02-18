/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "wasmtutorialstarter.h"
#include "logger.h"
#include "mozillavpn.h"

#include <emscripten/val.h>
#include <QUrlQuery>

namespace {
Logger logger(LOG_MAIN, "WasmTutorialStarter");
};

// static
void WasmTutorialStarter::initialize() {
  logger.debug() << "initializing";

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
    return;
  }

  MozillaVPN::instance()->getStarted();
}
