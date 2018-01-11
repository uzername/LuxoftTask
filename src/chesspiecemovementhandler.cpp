#include "chesspiecemovementhandler.h"


QString ChessPieceMovementHandler::getPathToAttackImage() const
{
    return pathToAttackImage;
}

void ChessPieceMovementHandler::setPathToAttackImage(const QString &value)
{
    pathToAttackImage = value;
}

QHash<int, QByteArray> ChessPieceMovementHandler::roleNames() const
{

}

QVariant ChessPieceMovementHandler::data(const QModelIndex &index, int role) const
{

}

int ChessPieceMovementHandler::rowCount(const QModelIndex &parent) const
{

}

ChessPieceMovementHandler::ChessPieceMovementHandler(QString in_pathToMovementImage, QString in_pathToAttackImage, QObject *parent): QAbstractListModel(parent) {
    this->pathToMovementImage = in_pathToMovementImage;
    this->pathToAttackImage = in_pathToAttackImage;
}

QString ChessPieceMovementHandler::getPathToMovementImage() const
{
    return pathToMovementImage;
}

void ChessPieceMovementHandler::setPathToMovementImage(const QString &value)
{
    pathToMovementImage = value;
}
