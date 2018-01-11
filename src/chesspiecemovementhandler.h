#ifndef CHESSPIECEMOVEMENTHANDLER_H
#define CHESSPIECEMOVEMENTHANDLER_H
#include <vector>
#include "chesspiecemetadata.h"
#include <QString>
#include <QAbstractListModel>

#include "mymodelroles.h"

/**
 * @brief The ChessPieceMovementHandler class is a model used to display available moves of selected chess piece. depends on QT utils
 * All evaluations are being performed by higher-grade ChessFieldPieces model, and this one receives only data to display
 */
class ChessPieceMovementHandler : public QAbstractListModel
{
protected:
    QString pathToMovementImage;
    QString pathToAttackImage;

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex & parent) const override;
public:
    Q_PROPERTY(QString movementImage READ getPathToMovementImage WRITE setPathToMovementImage NOTIFY movementImageChanged);
    Q_PROPERTY(QString attackImage READ attackImage WRITE setAttackImage NOTIFY attackImageChanged);
    ChessPieceMovementHandler(QString in_pathToMovementImage, QString in_pathToAttackImage, QObject* parent=0);
    QString getPathToMovementImage() const;
    void setPathToMovementImage(const QString &value);
    QString getPathToAttackImage() const;
    void setPathToAttackImage(const QString &value);
};

#endif // CHESSPIECEMOVEMENTHANDLER_H
