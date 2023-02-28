import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id:window
    property int gridSpacing: 8
    width: adjusted_size(1200, gridSpacing, ng.grid_size_y) +500
    height: adjusted_size(1200, gridSpacing, ng.grid_size_x) +500
    visible: true
    color: "#94999a"
    title: qsTr("Game of Life")

    function adjusted_size(size, spacing, nb_rows) {
        return nb_rows * Math.round((size-spacing)/nb_rows )+spacing
    }

    function choose_color(couleur ) {
        if (couleur===0){
            return "white"
        }
        if (couleur===1){
            return "black"
        }

    }

    Grid {
        id: grid
        rows: ng.get_x_size()
        columns: ng.get_y_size()
        spacing: gridSpacing


        width: window.width - (1+columns)*spacing
        height: window.height - (1+rows)*spacing-20
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: spacing
        anchors.topMargin: spacing
        Component.onCompleted: forceActiveFocus()

        Repeater {
                id: tiles

                model: ng.get_cell_number()
                delegate: Rectangle{
                    focus:true
                    width: Math.round(grid.width / grid.columns)
                    height: Math.round(grid.height / grid.rows)
                    color: choose_color(ng.value)

                }
    }


}
    Rectangle {
        width: window.width
        height: 20
        x: 0
        y: window.height-20
        color: "#ADACAC"
        Text {
            font.pixelSize: 15
            anchors.centerIn: parent
            text: "Next Generation"
            color: "white"
              }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                   ng.next_generation()
                       }
    }
}
}

