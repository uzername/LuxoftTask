#include "chessfieldpieces.h"

ChessFieldPieces::ChessFieldPieces()
{

}

ChessFieldPieces::ChessFieldPieces(ChessIntegerCoordType in_boardSize, QObject *parent = 0)
         : QAbstractListModel(parent)
{
    this->allChessPiecesDisplayed = std::vector<ChessPieceOnField>();
    this->internalBoardSize = in_boardSize;
}

ChessIntegerCoordType ChessFieldPieces::boardSize()
{
    return this->internalBoardSize;
}

void ChessFieldPieces::setInternalBoardSize(const ChessIntegerCoordType &value)
{
    internalBoardSize = value;
}

QHash<int, QByteArray> ChessFieldPieces::roleNames() const {
    QHash<int, QByteArray> names;
    names.insert(RoleImagePath, "display");
    names.insert(RolePositionX, "PositionX");
    names.insert(RolePositionY, "PositionY");
}

QVariant ChessFieldPieces::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
      return QVariant();
    }
    int indexNum = static_cast<int>(index.row());
    ChessPieceOnField &currentPiece = this->allChessPiecesDisplayed[indexNum];
    switch (role) {
        case (RoleImagePath): {
            return currentPiece.getPathToImage();
            break;
        }
        case (RolePositionX): {
            return currentPiece.getCurrentXonField();
            break;
        }
        case (RolePositionY): {
            return currentPiece.getCurrentYonField();
            break;
        }
        default: {
            return QVariant();
            break;
        }
    }
    return QVariant();
}

int ChessFieldPieces::rowCount(const QModelIndex &parent) const {
    return this->allChessPiecesDisplayed.size();
}

std::string ChessFieldPieces::getBoardPathToImage() const
{
    return boardPathToImage;
}

void ChessFieldPieces::setBoardPathToImage(const std::string &value)
{
    boardPathToImage = value;
}
void ChessFieldPieces::appendPieceOnField(ChessPieceOnField pieceToAppend) {
    this->allChessPiecesDisplayed.push_back(pieceToAppend);
}
//=============================================
ChessIntegerCoordType ChessPieceOnField::getCurrentXonField() const
{
    return currentXonField;
}

void ChessPieceOnField::setCurrentXonField(const ChessIntegerCoordType &value)
{
    currentXonField = value;
}

ChessIntegerCoordType ChessPieceOnField::getCurrentYonField() const
{
    return currentYonField;
}

void ChessPieceOnField::setCurrentYonField(const ChessIntegerCoordType &value)
{
    currentYonField = value;
}

std::string ChessPieceOnField::getPathToImage() const
{
    return pathToImage;
}

void ChessPieceOnField::setPathToImage(const std::string &value)
{
    pathToImage = value;
}
