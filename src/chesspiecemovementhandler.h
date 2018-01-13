#ifndef CHESSPIECEMOVEMENTHANDLER_H
#define CHESSPIECEMOVEMENTHANDLER_H
#include <vector>
#include "chesspiecemetadata.h"
#include <QString>
#include <QAbstractListModel>

#include "mymodelroles.h"
typedef enum displayMovementType {MOVEMENT_TYPE, ATTACK_TYPE} displayMovementType;
class displayMovementStructure {
public:
    ChessIntegerCoordType mvAtkPositionX;
    ChessIntegerCoordType mvAtkPositionY;
    displayMovementType mvAtkCurrentType;

};
/**
 * @brief The ChessPieceMovementHandler class is a model used to display available moves of selected chess piece. depends on QT utils
 * All evaluations are being performed by higher-grade ChessFieldPieces model, and this one receives only data to display
 */
class ChessPieceMovementHandler : public QAbstractListModel
        //it is hard to avoid 'use of deleted function'. To do this add Q_OBJECT, explicit to ctor def and define standard overloaded ctor (2 variants?)
        //and do not use '=' operator when creating instance
{
    Q_OBJECT
protected:
    QString pathToMovementImage;
    QString pathToAttackImage;

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex & parent) const override;

    std::vector<displayMovementStructure> allMvAtkStructures;
public:
    Q_PROPERTY(QString movementImage READ getPathToMovementImage WRITE setPathToMovementImage NOTIFY movementImageChanged);
    Q_PROPERTY(QString attackImage READ getPathToAttackImage WRITE setPathToAttackImage NOTIFY attackImageChanged);
    explicit ChessPieceMovementHandler(QString in_pathToMovementImage, QString in_pathToAttackImage, QObject* parent=0);
    explicit ChessPieceMovementHandler(QObject* parent=0);
    QString getPathToMovementImage() const;
    void setPathToMovementImage(const QString &value);
    QString getPathToAttackImage() const;
    void setPathToAttackImage(const QString &value);
    // https://stackoverflow.com/questions/3065154/undefined-reference-to-vtable
    // http://www.prog.org.ru/topic_15059_0.html (rerun qmake; 7 troubles - one clean build)
    //~ChessPieceMovementHandler();


    void setAllMvAtkStructures(const std::vector<displayMovementStructure> &value);
    void clearModelData();
signals:
    void movementImageChanged(QString);
    void attackImageChanged(QString);
};

#endif // CHESSPIECEMOVEMENTHANDLER_H
