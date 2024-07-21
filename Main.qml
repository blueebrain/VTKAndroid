import QtQuick
import com.vtk.example 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("VTKE")

    Rectangle {
      anchors.fill: parent
      color:"black"

      MyVtkItem {
        id: vtk
        anchors.fill: parent
        anchors.margins: 0
      }
    }
}
