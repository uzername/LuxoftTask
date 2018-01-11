#ifndef CHESSFIELDPIECES_H
#define CHESSFIELDPIECES_H

#include <QAbstractListModel>
#include "chesspiecemetadata.h"
#include "chesspiecemovementhandler.h"

#include <memory>

#include "mymodelroles.h"

/**
 * @brief The ChessPieceOnField class represents a single item on field. Independent from QT, may be worth moving to separate file
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

};
/**
 * @brief The ChessFieldPieces class reflects upon the gamefield and lies close to display.
 * This is a model. It is used to contain all the gamefield figures. Depends on QT items
 */
class ChessFieldPieces : public QAbstractListModel
{
    Q_OBJECT
public:
    // http://doc.qt.io/qt-5/properties.html
    Q_PROPERTY(/*ChessIntegerCoordType*/int boardSize READ boardSize WRITE setInternalBoardSize NOTIFY boardSizeChanged);
    // I am afraid you can't use std::string directly within QML, you have to convert to QString first.
    // https://stackoverflow.com/questions/17216627/using-stdstring-in-qml
    Q_PROPERTY(QString boardPathToImageBackground READ getBoardPathToImage WRITE setBoardPathToImage NOTIFY boardImageChanged);
    explicit ChessFieldPieces(ChessIntegerCoordType in_boardSize, QObject *parent = 0);
    explicit ChessFieldPieces(QObject *parent = 0);
    /**
     * @brief boardSize. should be int to avoid errors and crashes caused by Q_PROPERTY
     * @return the size of game board
     */
    /*ChessIntegerCoordType*/int boardSize();
    void setInternalBoardSize(const /*ChessIntegerCoordType*/int &value);
    void appendPieceOnField(ChessPieceOnField pieceToAppend);
    QString getBoardPathToImage() const;
    void setBoardPathToImage(const QString &value);
    /**
     * @brief signMovementAndAttackHandlingPact
     * @param in_chessPieceMovementHandler points to an instance where finalized info about allowed movement positions should be passed
     */
    void signMovementAndAttackHandlingPact(std::shared_ptr<ChessPieceMovementHandler> in_chessPieceMovementHandler);

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
    QString boardPathToImage;
    std::vector<ChessPieceOnField> allChessPiecesDisplayed;
    std::shared_ptr<ChessPieceMovementHandler> movementInfoModel;
signals:
    void boardSizeChanged(ChessIntegerCoordType);
    void boardImageChanged(std::string);
};

#endif // CHESSFIELDPIECES_H
