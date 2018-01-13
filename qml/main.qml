//constructed by Ancients
import QtQuick 2.7
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
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

    StateGroup {
        //this gnome switches the screens and buttons. He stands over here
    id: screenSwitchingGnome
    states: [
        //a common approach is to change buttons visibility on certain button's action, but:
        //usage of PropertyChanges enables an item's property values to be changed when it changes between states (experimental!)
            State {
                name: "screen1"
                PropertyChanges { target: startButton; visible: true  }
                PropertyChanges { target: stopButton; visible: false  }
                PropertyChanges { target: saveButton; visible: false  }
                PropertyChanges { target: loadButton; visible: true   }
                PropertyChanges { target: prevAndNext; visible: false }
            },
            State {
                name: "screen2"
                PropertyChanges { target: startButton; visible: false  }
                PropertyChanges { target: stopButton;  visible: true   }
                PropertyChanges { target: saveButton;  visible: true   }
                PropertyChanges { target: loadButton;  visible: false  }
                PropertyChanges { target: prevAndNext; visible: false  }
            },
            State {
                name: "screen3"
                PropertyChanges { target: startButton; visible: true }
                PropertyChanges { target: stopButton;  visible: false  }
                PropertyChanges { target: saveButton;  visible: false }
                PropertyChanges { target: loadButton;  visible: true  }
                PropertyChanges { target: prevAndNext; visible: true  }
            }
        ]
    }

    Item {
      id: gameBoard
      x: 0
      y: 0
      width : squareSize*mylogic.boardSize
      height: squareSize*mylogic.boardSize
      
      Image {
        id: chessBoardImg
        source: mylogic.boardPathToImageBackground // "/images/chess_board.jpg"
        height: gameBoard.height
        width: gameBoard.width
      }
      //repeater component is used to bind logic and components. Actually it is used to handle lists and sequences of objects
      // http://doc.qt.io/qt-5/qml-qtquick-repeater.html
      //this approach is unnatural. A better way: http://doc.qt.io/qt-5/qtqml-cppintegration-overview.html
      //well, that kind of model and repeater may actually work.
      Repeater {//display available movement and attack possibilities
          model: mvatklogic
          Image {
               height: squareSize
               width: squareSize
               //taken from RoleNames of model
               source: mvatk_display
               x: squareSize*mvatk_PositionX
               y: squareSize*mvatk_PositionY
          }

      }
      Repeater {
         //sort of qlistmodel
        model: mylogic

        Image { //a chess piece on gamefield
          height: squareSize
          width : squareSize
           //PositionX, PositionY, display are taken from model (check out Role Names).
           //Only the most basic types are allowed for usage by model due to "QMetaProperty::read: Unable to handle unregistered datatype" error
          x: squareSize*PositionX
          y: squareSize*PositionY

          source: display /*images[type].imgPath*/
          //create mouseArea to handle mouse events
          MouseArea {
            anchors.fill: parent
            drag.target: parent

            property int startX: 0
            property int startY: 0

            onPressed: {
              startX = parent.x;
              startY = parent.y;
                var preparedX = startX / squareSize;
                var preparedY = startY / squareSize;
                mylogic.activateDisplayAvailableMoves(preparedX, preparedY);

            }

            onReleased: {
              var fromX = startX / squareSize;
              var fromY = startY / squareSize;
              var toX   = (parent.x + mouseX) / squareSize;
              var toY   = (parent.y + mouseY) / squareSize;
              mylogic.activateClearModelData();
              //if (!logic.move(fromX, fromY, toX, toY)) {
                parent.x = startX;
                parent.y = startY;
              //}
            }
          }
        }

      }

    }
    Column {
        anchors.left: gameBoard.right
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
    Button {
      id: startButton

      visible: true
      text: "Start"
       anchors.right: parent.right
       anchors.left: parent.left
      onClicked: {
        //logic.clear();

        if ((screenSwitchingGnome.state == "screen1")||(screenSwitchingGnome.state == "")||(screenSwitchingGnome.state == "screen3")) {
            screenSwitchingGnome.state = "screen2";
        }

      }
    }

    Button {
        id: stopButton
        visible: false;
        text: "Stop"
        anchors.right: parent.right
        anchors.left: parent.left
        onClicked: {

          if (screenSwitchingGnome.state == "screen2") {
             screenSwitchingGnome.state = "screen1";

          }

        }

    }
    Button {
       id: loadButton
       text:"Load"
       visible:true;
       anchors.right: parent.right
       anchors.left: parent.left

       onClicked: {
          if ((screenSwitchingGnome.state == "")||(screenSwitchingGnome.state == "screen1")||(screenSwitchingGnome.state == "screen3")) {
              screenSwitchingGnome.state = "screen3"
          }
       }

    }
    Button {
        anchors.right: parent.right
        anchors.left: parent.left

       id: saveButton
       text:"Save"
       visible:false;
       onClicked: {

       }

    }
    Row {
        id:prevAndNext
        anchors.right: parent.right
        anchors.left: parent.left
        visible:false
        Button {
           id: prevButton
           text: "Prev"
        }
        Button {
           id: nextButton
           text: "Next"
        }
    }

    }

}
