import QtQuick 2.13
import QtQuick.Controls 2.5

import "../Style" as S
import "../Core" as C

Popup {
    property int freqId: -1
    property int freqGroup: -1
    
    signal s_generate(date begin, date end)
}
