#ifndef CHESSFIELDPIECES_H
#define CHESSFIELDPIECES_H

#include <QAbstractListModel>
#include "chesspiecemetadata.h"
#include "chesspiecemovementhandler.h"

#include <memory>

#include "mymodelroles.h"
   //BITMAPS FOR movementPerformed VARIABLE for PAWN (BOTH BLACK AND WHITE).
#include "moredefs.h"

//======================================
/**
 * @brief The ChessPieceOnField class represents a single item on field. Independent from QT, may be worth moving to separate file
 */
class ChessPieceOnField {
protected:
    ChessPieceMetadataBehavior* instBehaviorChessPiece;
    //std::shared_ptr<ChessPieceMetadataDisplay> instDisplayChessPiece;
    std::string pathToImage;
    ChessIntegerCoordType currentXonField;
    ChessIntegerCoordType currentYonField;
    /**
     * @brief movementPerformed - this number contains metadata for pawn. Why create subclass and fiddle with it if you can contain all the data in uint8_t?
     */
    uint8_t movementPerformed;

    ChessPieceSideTypes currentSideType;
public:
    ChessPieceOnField(ChessPieceMetadataBehavior* in_Behavior, std::string in_pathToImage);
    std::string getPathToImage() const;
    void setPathToImage(const std::string &value);
    ChessIntegerCoordType getCurrentXonField() const;
    void setCurrentXonField(const ChessIntegerCoordType &value);
    ChessIntegerCoordType getCurrentYonField() const;
    void setCurrentYonField(const ChessIntegerCoordType &value);

    ChessPieceMetadataBehavior *getInstBehaviorChessPiece() const;
    void setMovementPerformed(const uint8_t &value);
    uint8_t* getMovementPerformed();
    ChessPieceSideTypes getCurrentSideType() const;
    void setCurrentSideType(const ChessPieceSideTypes &value);
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
    void signMovementAndAttackHandlingPact(ChessPieceMovementHandler *in_chessPieceMovementHandler);
    // add Q_INVOKABLE to display items on the field http://doc.qt.io/qt-5/qtqml-cppintegration-exposecppattributes.html
    /**
     * @brief activateDisplayAvailableMoves fills up the array of ChessPieceMovementHandler* movementInfoModel previously set by signMovementAndAttackHandlingPact
     * @param in_X coordinate of chess piece
     * @param in_Y coordinate of chess piece
     */
    Q_INVOKABLE void activateDisplayAvailableMoves (int in_X, int in_Y);
    /**
     * @brief findByPosition
     * @param in_X
     * @param in_Y
     * @return pointer to chess figurine on this chess cell.
     */
    ChessPieceOnField* findByPosition(int in_X, int in_Y);
    ChessPieceOnField* findByPosition(int in_X, int in_Y, unsigned int *inout_indexptr);
    /**
      * @brief Clears available moves. this routine may be called from child model, but let's keep all the calls here
      */
    Q_INVOKABLE void activateClearModelData();
    /**
      * @brief Performs movement of chess piece on board. All data here are absolute integer coordinates of cells
      * It is possible to use data in movementInfoModel, which have been filled in activateDisplayAvailableMoves
      * Changes data in model, if acceptable
      * @returns 0 if movement inavailable; 1 if movement may be performed; 2 if error happened
      */
    Q_INVOKABLE int move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE void fillGameField();
    Q_INVOKABLE void clearGameField();
    ChessPieceSideTypes getCurrentMove() const;
    void setCurrentMove(const ChessPieceSideTypes &value);
    void increaseCurrentMove();

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
    ChessPieceMovementHandler* movementInfoModel;
    ChessPieceSideTypes currentMove;
    /**
     * @brief setFieldPieceBitMapVariable - used to set value of movementPerformed for some chess piece. UNUSED!
     */
    void setFieldPieceBitMapVariable(ChessPieceOnField* in_piece);
signals:
    void boardSizeChanged(ChessIntegerCoordType);
    void boardImageChanged(std::string);
};

#endif // CHESSFIELDPIECES_H
