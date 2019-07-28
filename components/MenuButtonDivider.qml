import QtQuick 2.9

import "." as NejcoinComponents
import "effects/" as NejcoinEffects

Rectangle {
    color: NejcoinComponents.Style.appWindowBorderColor
    height: 1

    NejcoinEffects.ColorTransition {
        targetObj: parent
        blackColor: NejcoinComponents.Style._b_appWindowBorderColor
        whiteColor: NejcoinComponents.Style._w_appWindowBorderColor
    }
}
