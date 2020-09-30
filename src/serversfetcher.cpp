/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "serversfetcher.h"
#include "mozillavpn.h"
#include "networkrequest.h"
#include "servercountrymodel.h"

void ServersFetcher::run(MozillaVPN *vpn)
{
    NetworkRequest *request = NetworkRequest::createForServers(vpn);

    connect(request, &NetworkRequest::requestFailed, [this](QNetworkReply::NetworkError error) {
        qDebug() << "Failed to retrieve servers";
        emit failed(error);
    });

    connect(request, &NetworkRequest::requestCompleted, [this](const QByteArray &data) {
        qDebug() << "Servers obtained";
        emit completed(data);
    });
}
