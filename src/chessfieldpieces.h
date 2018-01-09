#ifndef CHESSFIELDPIECES_H
#define CHESSFIELDPIECES_H

#include <QAbstractListModel>
#include "chesspiecemetadata.h"
/**
 * @brief The ChessFieldPieces class reflects upon the gamefield and lies close to display.
 * This is a model. It is used to contain all the gamefield figures
 */
class ChessFieldPieces : public QAbstractListModel
{
    Q_OBJECT
public:
    // http://doc.qt.io/qt-5/properties.html
    Q_PROPERTY(ChessIntegerCoordType boardSize READ boardSize CONSTANT)
    ChessFieldPieces(ChessIntegerCoordType in_boardSize, QObject *parent);
    ChessIntegerCoordType boardSize();
private:
    ChessIntegerCoordType internalBoardSize;

};

#endif // CHESSFIELDPIECES_H
