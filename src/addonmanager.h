/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef ADDONMANAGER_H
#define ADDONMANAGER_H

#include "addons/addon.h"  // required for the signal

#include <QMap>
#include <QAbstractListModel>

class QDir;

class AddonManager final : public QAbstractListModel {
  Q_OBJECT
  Q_DISABLE_COPY_MOVE(AddonManager)

 public:
  enum ModelRoles {
    AddonRole = Qt::UserRole + 1,
  };

  static AddonManager* instance();

  ~AddonManager();

  void updateIndex(const QByteArray& index);

  void storeAndLoadAddon(const QByteArray& addonData, const QString& addonId,
                         const QByteArray& sha256);

  bool loadManifest(const QString& addonManifestFileName,
                    const QByteArray& sha256);

  void unload(const QString& addonId);

  void retranslate();

  void forEach(std::function<void(Addon* addon)>&& callback);

 private:
  explicit AddonManager(QObject* parent);

  void initialize();

  bool validateIndex(const QByteArray& index);
  bool validateAndLoad(const QString& addonId, const QByteArray& sha256,
                       bool checkSha256 = true);

  static bool addonDir(QDir* dir);
  static QByteArray readIndex();
  static void writeIndex(const QByteArray& index);

  static void removeAddon(const QString& addonId);

  // QAbstractListModel methods

  QHash<int, QByteArray> roleNames() const override;

  int rowCount(const QModelIndex&) const override;

  QVariant data(const QModelIndex& index, int role) const override;

 signals:
  void runAddon(Addon* addon);

 private:
  struct AddonData {
    QByteArray m_sha256;
    QString m_addonId;
    Addon* m_addon;
  };

  QMap<QString, AddonData> m_addons;
};

#endif  // ADDONMANAGER_H
