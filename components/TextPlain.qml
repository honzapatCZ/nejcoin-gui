import QtQuick 2.9

import "." as NejcoinComponents
import "effects/" as NejcoinEffects

Text {
    // When using this component, please note that if you use a color different
    // than `defaultFontColor`, you are required to also define `themeTransitionXColor`.
    // If you do not set these the component will receive the wrong color after a transition.
    // If you do not want to set these, use `themeTransition: false`.
    id: root
    property bool themeTransition: true
    property string themeTransitionBlackColor: ""
    property string themeTransitionWhiteColor: ""
    font.family: NejcoinComponents.Style.fontMedium.name
    font.bold: false
    font.pixelSize: 14
    textFormat: Text.PlainText

    NejcoinEffects.ColorTransition {
        enabled: root.themeTransition
        themeTransition: root.themeTransition
        targetObj: root
        duration: 750
        blackColor: root.themeTransitionBlackColor !== "" ? root.themeTransitionBlackColor : NejcoinComponents.Style._b_defaultFontColor
        whiteColor: root.themeTransitionWhiteColor !== "" ? root.themeTransitionWhiteColor : NejcoinComponents.Style._w_defaultFontColor
    }
}
