import QtQuick 2.9

import "../components" as NejcoinComponents

TextEdit {
    color: NejcoinComponents.Style.defaultFontColor
    font.family: NejcoinComponents.Style.fontRegular.name
    selectionColor: NejcoinComponents.Style.textSelectionColor
    wrapMode: Text.Wrap
    readOnly: true
    selectByMouse: true
    // Workaround for https://bugreports.qt.io/browse/QTBUG-50587
    onFocusChanged: {
        if(focus === false)
            deselect()
    }
}
