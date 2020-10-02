/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.11

import Mozilla.VPN 1.0

import "../components"
import "../themes/themes.js" as Theme

Item {
    id: updatePanel
    state: VPN.updateRecommended ? "recommended" : "required"

    states: [
        State {
            name: "recommended"
            PropertyChanges {
                target: contentWrapper
                logo: "../resources/update-recommended.svg"
                logoTitle: qsTr("Update recomended")
                logoSubtitle: qsTr("Please update the app before you\ncontinue to use the VPN")
            }
            PropertyChanges {
                target: notNow
                visible: true
            }
            PropertyChanges {
                target: manageAccount
                visible: false
            }
            PropertyChanges {
                target: signOff
                visible: false
            }
        },

        State {
            name: "required"
            PropertyChanges {
                target: contentWrapper
                logo: "../resources/update-required.svg"
                logoTitle: qsTr("Update required")
                logoSubtitle: qsTr("We detected and fixed a serious bug.\nYou must update your app.")
            }
            PropertyChanges {
                target: notNow
                visible: false
            }
            PropertyChanges {
                target: manageAccount
                visible: VPN.userAuthenticated
            }
            PropertyChanges {
                target: signOff
                visible: VPN.userAuthenticated
            }
        }
    ]

    VPNPanel {
        id: contentWrapper
    }

    // TODO: Your connection will not be secure while you update.

    VPNButton {
        width: 282
        text: qsTr("Update now")
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        y: 300
        radius: 4
        onClicked: VPN.openLink(VPN.LinkUpdate)
    }

    VPNFooterLink {
        id: notNow
        labelText: qsTr("Not now")
        onClicked: mainStackView.pop()
    }

    VPNFooterLink {
        id: manageAccount
        labelText: qsTr("Manage account")
        onClicked: VPN.openLink(VPN.LinkAccount)
    }

    Text {
        id: signOff
        anchors.top: manageAccount.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: Theme.fontSize
        font.family: vpnFontInter.name
        text: qsTr("Sign off")
        color: "red"

        MouseArea {
            anchors.fill: parent
            onClicked: VPNController.logout()
            cursorShape: "PointingHandCursor"
            hoverEnabled: true
        }
    }
}
