//#include "logic.h"
#include "chessfieldpieces.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
void fillGameField(ChessFieldPieces *instGameField);
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    //Logic logic;
    ChessFieldPieces gamefieldCurrentState = ChessFieldPieces(8);

    QQmlApplicationEngine engine;
    //engine.rootContext()->setContextProperty("logic", &logic);
    engine.rootContext()->setContextProperty("mylogic", &gamefieldCurrentState);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
void fillGameField(ChessFieldPieces *instGameField) {
    //instGameField->appendPieceOnField();
}
