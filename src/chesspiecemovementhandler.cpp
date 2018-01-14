#include "chesspiecemovementhandler.h"


QString ChessPieceMovementHandler::getPathToAttackImage() const
{
    return pathToAttackImage;
}

void ChessPieceMovementHandler::setPathToAttackImage(const QString &value)
{
    pathToAttackImage = value;
}

void ChessPieceMovementHandler::removeItemByIndex(unsigned int in_index) {
    // https://stackoverflow.com/questions/875103/how-do-i-erase-an-element-from-stdvector-by-index
    if (in_index>=this->allMvAtkStructures.size()) { return; }
    this->allMvAtkStructures.erase(this->allMvAtkStructures.begin()+in_index);
}

//ChessPieceMovementHandler::~ChessPieceMovementHandler() { }

QHash<int, QByteArray> ChessPieceMovementHandler::roleNames() const
{
    QHash<int, QByteArray> names;
    names.insert(RoleImagePath, "mvatk_display");
    names.insert(RolePositionX, "mvatk_PositionX");
    names.insert(RolePositionY, "mvatk_PositionY");
    return names;
}

QVariant ChessPieceMovementHandler::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
      return QVariant();
    }
    int indexNum = static_cast<int>(index.row());
    displayMovementStructure currentPiece = this->allMvAtkStructures[indexNum];
    switch (role) {
        case (RoleImagePath): {
            if (currentPiece.mvAtkCurrentType == MOVEMENT_TYPE) {
                return this->pathToMovementImage;
            }
            if (currentPiece.mvAtkCurrentType == ATTACK_TYPE) {
                return this->pathToAttackImage;
            }
            break;
        }
        case (RolePositionX): {
            return currentPiece.mvAtkPositionX;
            break;
        }
        case (RolePositionY): {
            return currentPiece.mvAtkPositionY;
            break;
        }
        default: {
            return QVariant();
            break;
        }
    }
    return QVariant();
}

int ChessPieceMovementHandler::rowCount(const QModelIndex &parent) const
{
    return this->allMvAtkStructures.size();
}

void ChessPieceMovementHandler::setAllMvAtkStructures(const std::vector<displayMovementStructure> &value)
{
    beginResetModel();
    allMvAtkStructures = value;
    endResetModel();
}

std::vector<displayMovementStructure>::iterator ChessPieceMovementHandler::getMvAtkStructuresIteratorBegin() {
    return this->allMvAtkStructures.begin();
}

std::vector<displayMovementStructure>::iterator ChessPieceMovementHandler::getMvAtkStructuresIteratorEnd() {
    return this->allMvAtkStructures.end();
}

void ChessPieceMovementHandler::clearModelData()
{
    beginResetModel();
    allMvAtkStructures.clear();
    endResetModel();
}

ChessPieceMovementHandler::ChessPieceMovementHandler(QString in_pathToMovementImage, QString in_pathToAttackImage, QObject *parent): QAbstractListModel(parent) {
    this->allMvAtkStructures = std::vector<displayMovementStructure>();
    this->pathToMovementImage = in_pathToMovementImage;
    this->pathToAttackImage = in_pathToAttackImage;
}

ChessPieceMovementHandler::ChessPieceMovementHandler(QObject *parent): QAbstractListModel(parent)
{

}

QString ChessPieceMovementHandler::getPathToMovementImage() const
{
    return pathToMovementImage;
}

void ChessPieceMovementHandler::setPathToMovementImage(const QString &value)
{
    pathToMovementImage = value;
}
