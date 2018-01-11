//#include "logic.h"
#include "chessfieldpieces.h"
#include "chesspiecemovementhandler.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
void fillGameField(ChessFieldPieces *instGameField);
/**
 * @brief main. The following app is used to show some chess interface functionality.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    //Logic logic;

    ChessFieldPieces gamefieldCurrentState;
      gamefieldCurrentState.setInternalBoardSize(8);
      gamefieldCurrentState.setBoardPathToImage("/images/chess_board.jpg");
    fillGameField(&gamefieldCurrentState);



    QQmlApplicationEngine engine;
    //engine.rootContext()->setContextProperty("logic", &logic);
    engine.rootContext()->setContextProperty("mylogic", &gamefieldCurrentState);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
void fillGameField(ChessFieldPieces *instGameField) {
    initBehaviors();
    ChessPieceOnField whiteKing = ChessPieceOnField(instKingBehavior, "/images/white_king.svg");
    whiteKing.setCurrentXonField(1); whiteKing.setCurrentYonField(2);
    instGameField->appendPieceOnField(whiteKing);

}
