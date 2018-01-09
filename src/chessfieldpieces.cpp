#include "chessfieldpieces.h"

ChessFieldPieces::ChessFieldPieces()
{

}

ChessFieldPieces::ChessFieldPieces(ChessIntegerCoordType in_boardSize, QObject *parent)
         : QAbstractListModel(parent)
{
    this->internalBoardSize = in_boardSize;
}

ChessIntegerCoordType ChessFieldPieces::boardSize()
{
    return this->internalBoardSize;
}
