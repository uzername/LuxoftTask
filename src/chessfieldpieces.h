#ifndef CHESSFIELDPIECES_H
#define CHESSFIELDPIECES_H

#include <QAbstractListModel>
#include "chesspiecemetadata.h"
#include <memory>
/**
 * @brief The ChessFieldPieces class reflects upon the gamefield and lies close to display.
 * This is a model. It is used to contain all the gamefield figures. Depends on QT items
 */
enum Roles {
    RoleImagePath = Qt::DisplayRole,
    RolePositionX,
    RolePositionY,
};

};
class ChessFieldPieces : public QAbstractListModel
{
    Q_OBJECT
public:
    // http://doc.qt.io/qt-5/properties.html
    Q_PROPERTY(ChessIntegerCoordType boardSize READ boardSize WRITE setInternalBoardSize NOTIFY boardSizeChanged);
    Q_PROPERTY(std::string boardPathToImageBackground READ getBoardPathToImage WRITE setBoardPathToImage NOTIFY boardImageChanged);
    ChessFieldPieces(ChessIntegerCoordType in_boardSize, QObject *parent);
    /**
     * @brief boardSize
     * @return the size of game board
     */
    ChessIntegerCoordType boardSize();
    void setInternalBoardSize(const ChessIntegerCoordType &value);
    void appendPieceOnField(ChessPieceOnField pieceToAppend);
    std::string getBoardPathToImage() const;
    void setBoardPathToImage(const std::string &value);

protected:
    //see "Model/View Programming" and "QAbstractListModel class"
    // When subclassing QAbstractListModel, you must provide implementations of the rowCount() and data() functions.
    // If your model is used within QML and requires roles other than the default ones provided by the roleNames() function, you must override it.
    //Items in a model can perform various roles for other components, allowing different kinds of data to be supplied for different situations.
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex & parent) const override;

private:
    ChessIntegerCoordType internalBoardSize;
    std::string boardPathToImage;
    std::vector<ChessPieceOnField> allChessPiecesDisplayed;
signals:
    void boardSizeChanged(ChessIntegerCoordType);
    void boardImageChanged(std::string);
};
/**
 * @brief The ChessPieceOnField class represents a single item on field. Independent from QT
 */
class ChessPieceOnField {
protected:
    std::shared_ptr<ChessPieceMetadataBehavior> instBehaviorChessPiece;
    //std::shared_ptr<ChessPieceMetadataDisplay> instDisplayChessPiece;
    std::string pathToImage;
    ChessIntegerCoordType currentXonField;
    ChessIntegerCoordType currentYonField;
public:
    ChessPieceOnField(std::shared_ptr<ChessPieceMetadataBehavior> in_Behavior, std::string in_pathToImage);
    std::string getPathToImage() const;
    void setPathToImage(const std::string &value);
    ChessIntegerCoordType getCurrentXonField() const;
    void setCurrentXonField(const ChessIntegerCoordType &value);
    ChessIntegerCoordType getCurrentYonField() const;
    void setCurrentYonField(const ChessIntegerCoordType &value);

}
#endif // CHESSFIELDPIECES_H
