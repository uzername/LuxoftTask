import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
//SomeName: this qml is useless! It is declarative, just like Prolog! I am being forced to go with that!
ApplicationWindow {
    title: qsTr("Chess")
    visible: true
    width: 800
    height: 600
    
    property int squareSize: 70
    //Use that somewhere else, but not here
/*
    property var images: [
      {'imgPath' : "/images/white_pawn.svg"},
      {'imgPath' : "/images/black_pawn.svg"},
    ]
*/
    Item {
      id: gameBoard
      x: 0
      y: 0
      width : logic.boardSize * squareSize
      height: logic.boardSize * squareSize
      
      Image {
        source: "/images/chess_board.jpg"
        height: gameBoard.height
        width: gameBoard.width
      }
      //repeater component is used to bind logic and components. Actually it is used to handle lists and sequences of objects
      // http://doc.qt.io/qt-5/qml-qtquick-repeater.html
      //this approach is unnatural. A better way: http://doc.qt.io/qt-5/qtqml-cppintegration-overview.html
      //well, that kind of model and repeater may actually work.
/*
      Repeater {
        model: logic
*/
        Image { //a chess piece on gamefield
          height: squareSize
          width : squareSize

          x: squareSize * positionX
          y: squareSize * positionY

          //source: images[type].imgPath
          //create mouseArea to handle mouse events
          MouseArea {
            anchors.fill: parent
            drag.target: parent

            property int startX: 0
            property int startY: 0

            onPressed: {
              startX = parent.x;
              startY = parent.y;
            }

            onReleased: {
              var fromX = startX / squareSize;
              var fromY = startY / squareSize;
              var toX   = (parent.x + mouseX) / squareSize;
              var toY   = (parent.y + mouseY) / squareSize;

              //if (!logic.move(fromX, fromY, toX, toY)) {
                parent.x = startX;
                parent.y = startY;
              //}
            }
          }
        }
     /* } */
    }

    Button {
      id: startButton
      anchors.left: gameBoard.right
      anchors.right: parent.right
      anchors.leftMargin: 10
      anchors.rightMargin: 10
      
      text: "Clear"

      onClicked: {
        //logic.clear();
      }
    }
}
