// Copyright (c) 2019-2019, Nejcraft
// Copyright (c) 2014-2019, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import QtQuick 2.9
import QtQuick.Layouts 1.1

import "../components" as NejcoinComponents

ColumnLayout {
    id: item

    Layout.fillWidth: true

    property alias text: input.text
    property alias labelText: inputLabel.text
    property alias labelButtonText: labelButton.text
    property alias placeholderText: placeholderLabel.text

    property int inputPaddingLeft: 10
    property int inputPaddingRight: 10
    property int inputPaddingTop: 10
    property int inputPaddingBottom: 10
    property int inputRadius: 4

    property bool placeholderCenter: false
    property string placeholderFontFamily: NejcoinComponents.Style.fontRegular.name
    property bool placeholderFontBold: false
    property int placeholderFontSize: 18
    property string placeholderColor: NejcoinComponents.Style.defaultFontColor
    property real placeholderOpacity: 0.35

    property bool borderDisabled: false
    property string borderColor: {
        if(input.error && input.text !== ""){
            return NejcoinComponents.Style.inputBorderColorInvalid;
        } else if(input.activeFocus){
            return NejcoinComponents.Style.inputBorderColorActive;
        } else {
            return NejcoinComponents.Style.inputBorderColorInActive;
        }
    }

    property bool error: false

    property string labelFontColor: NejcoinComponents.Style.defaultFontColor
    property bool labelFontBold: false
    property int labelFontSize: 16
    property bool labelButtonVisible: false

    property string fontColor: NejcoinComponents.Style.defaultFontColor
    property bool fontBold: false
    property int fontSize: 16

    property bool mouseSelection: true
    property alias readOnly: input.readOnly
    property bool copyButton: false
    property bool pasteButton: false
    property var onPaste: function(clipboardText) {
        item.text = clipboardText;
    }
    property bool showingHeader: labelText != "" || copyButton || pasteButton
    property var wrapMode: Text.NoWrap
    property alias addressValidation: input.addressValidation
    property string backgroundColor: "" // mock

    property alias inlineButton: inlineButtonId
    property bool inlineButtonVisible: false

    signal labelButtonClicked();
    signal inputLabelLinkActivated();
    signal editingFinished();

    spacing: 0
    Rectangle {
        id: inputLabelRect
        color: "transparent"
        Layout.fillWidth: true
        height: (inputLabel.height + 10)
        visible: showingHeader ? true : false

        NejcoinComponents.TextPlain {
            id: inputLabel
            anchors.top: parent.top
            anchors.left: parent.left
            font.family: NejcoinComponents.Style.fontRegular.name
            font.pixelSize: item.labelFontSize
            font.bold: labelFontBold
            textFormat: Text.RichText
            color: item.labelFontColor
            onLinkActivated: inputLabelLinkActivated()

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.NoButton
                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
            }
        }

        RowLayout {
            anchors.right: parent.right
            spacing: 16

            NejcoinComponents.LabelButton {
                id: labelButton
                onClicked: labelButtonClicked()
                visible: labelButtonVisible
            }

            NejcoinComponents.LabelButton {
                id: copyButtonId
                visible: copyButton && input.text !== ""
                text: qsTr("Copy") + translationManager.emptyString
                onClicked: {
                    if (input.text.length > 0) {
                        console.log("Copied to clipboard");
                        clipboard.setText(input.text);
                        appWindow.showStatusMessage(qsTr("Copied to clipboard"), 3);
                    }
                }
            }

            NejcoinComponents.LabelButton {
                id: pasteButtonId
                onClicked: item.onPaste(clipboard.text())
                text: qsTr("Paste") + translationManager.emptyString
                visible: pasteButton
            }
        }
    }

    NejcoinComponents.InputMulti {
        id: input
        readOnly: false
        addressValidation: false
        Layout.fillWidth: true
        
        leftPadding: item.inputPaddingLeft
        rightPadding: item.inputPaddingRight
        topPadding: item.inputPaddingTop
        bottomPadding: item.inputPaddingBottom

        wrapMode: item.wrapMode
        fontSize: item.fontSize
        fontBold: item.fontBold
        fontColor: item.fontColor
        mouseSelection: item.mouseSelection
        onEditingFinished: item.editingFinished()
        error: item.error

        NejcoinComponents.TextPlain {
            id: placeholderLabel
            visible: input.text ? false : true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            opacity: item.placeholderOpacity
            color: item.placeholderColor
            font.family: item.placeholderFontFamily
            font.bold: item.placeholderFontBold
            font.pixelSize: item.placeholderFontSize
            text: ""
            z: 3
        }

        Rectangle {
            color: "transparent"
            border.width: 1
            border.color: item.borderColor
            radius: item.inputRadius
            anchors.fill: parent
            visible: !item.borderDisabled
        }

        NejcoinComponents.InlineButton {
            id: inlineButtonId
            visible: (inlineButtonId.text || inlineButtonId.icon) && inlineButtonVisible ? true : false
            anchors.right: parent.right
            anchors.rightMargin: 8
        }
    }
}
