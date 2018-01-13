//#include "logic.h"
#include "chessfieldpieces.h"
#include "chesspiecemovementhandler.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
void fillGameField(ChessFieldPieces *instGameField);
void globalCleanup();
ChessPieceMovementHandler* gamefieldMovementHandlerPtr;
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

    ChessPieceMovementHandler gamefieldMovementHandler(QString("/images/movement.svg"), QString("/images/attack.svg"));
    /*
    ChessPieceMovementHandler gamefieldMovementHandler;
    gamefieldMovementHandler.setPathToAttackImage(QString("/images/attack.svg"));
    gamefieldMovementHandler.setPathToAttackImage(QString("/images/movement.svg"));
    */
    //  gamefieldMovementHandlerPtr = (&gamefieldMovementHandler);
    gamefieldCurrentState.signMovementAndAttackHandlingPact(&gamefieldMovementHandler);

    QQmlApplicationEngine engine;
    //engine.rootContext()->setContextProperty("logic", &logic);
    engine.rootContext()->setContextProperty("mylogic", &gamefieldCurrentState);
    engine.rootContext()->setContextProperty("mvatklogic", &gamefieldMovementHandler);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    int exitcode = app.exec();
    //globalCleanup();
    deinitBehaviors();
    return exitcode;
}
void fillGameField(ChessFieldPieces *instGameField) {
    initBehaviors();
    ChessPieceOnField whiteKing = ChessPieceOnField(instKingBehavior, "/images/white_king.svg");
    whiteKing.setCurrentXonField(1); whiteKing.setCurrentYonField(2);
    instGameField->appendPieceOnField(whiteKing);

    ChessPieceOnField blackPawn = ChessPieceOnField(instBlackPawnBehavior, "/images/black_pawn.svg");
    blackPawn.setCurrentXonField(2); blackPawn.setCurrentYonField(2);
    instGameField->appendPieceOnField(blackPawn);
}

void globalCleanup() {
    if (gamefieldMovementHandlerPtr != Q_NULLPTR) {
        delete gamefieldMovementHandlerPtr;
    }
}

