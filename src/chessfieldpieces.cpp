#include "chessfieldpieces.h"

ChessFieldPieces::ChessFieldPieces(ChessIntegerCoordType in_boardSize, QObject *parent)
         : QAbstractListModel(parent)
{
    this->allChessPiecesDisplayed = std::vector<ChessPieceOnField>();
    this->internalBoardSize = in_boardSize;

    this->currentMove = FIRST_MOVE_SIDE;
}

ChessFieldPieces::ChessFieldPieces(QObject *parent) : QAbstractListModel(parent)
{

}

/*ChessIntegerCoordType*/int ChessFieldPieces::boardSize()
{
    return this->internalBoardSize;
}

void ChessFieldPieces::setInternalBoardSize(const int &value)
{
    internalBoardSize = value;
}

QHash<int, QByteArray> ChessFieldPieces::roleNames() const {
    QHash<int, QByteArray> names;
    names.insert(RoleImagePath, "display");
    names.insert(RolePositionX, "PositionX");
    names.insert(RolePositionY, "PositionY");
    return names;
}

QVariant ChessFieldPieces::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
      return QVariant();
    }
    int indexNum = static_cast<int>(index.row());
    ChessPieceOnField currentPiece = this->allChessPiecesDisplayed[indexNum];
    switch (role) {
        case (RoleImagePath): {
            return QString::fromStdString(currentPiece.getPathToImage());
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

void ChessFieldPieces::setCurrentMove(const ChessPieceSideTypes &value)
{
    currentMove = value;
}

void ChessFieldPieces::increaseCurrentMove() {
    if (currentMove == LAST_MOVE_SIDE) { currentMove = FIRST_MOVE_SIDE;
    } else {
        currentMove = ChessPieceSideTypes(currentMove+1);
    }
}

ChessPieceSideTypes ChessFieldPieces::getCurrentMove() const
{
    return currentMove;
}

QString ChessFieldPieces::getBoardPathToImage() const
{
    return boardPathToImage;
}

void ChessFieldPieces::setBoardPathToImage(const QString &value)
{
    boardPathToImage = value;
}

void ChessFieldPieces::signMovementAndAttackHandlingPact(ChessPieceMovementHandler* in_chessPieceMovementHandler) {
    this->movementInfoModel = in_chessPieceMovementHandler;

}

void ChessFieldPieces::activateDisplayAvailableMoves(int in_X, int in_Y)
{
    if (this->movementInfoModel == nullptr) {
        return;
    }
    ChessPieceOnField* instChessPiece = this->findByPosition(in_X, in_Y);
    if (this->getCurrentMove()!=instChessPiece->getCurrentSideType()) {
        return;
    }
    if (instChessPiece!=nullptr) {
        ChessPieceMetadataBehavior* theBehaviorToConvert = instChessPiece->getInstBehaviorChessPiece();
        theBehaviorToConvert->performActionsBeforeMovement((void*)(instChessPiece->getMovementPerformed()));
        std::vector<displayMovementStructure> currentStructureListMovements = std::vector<displayMovementStructure>();
        // https://stackoverflow.com/questions/2754650/getting-value-of-stdlistiterator-to-pointer
        // https://stackoverflow.com/questions/409348/iteration-over-stdvector-unsigned-vs-signed-index-variable
        for (std::vector<ChessPiecePointPattern>::iterator it = theBehaviorToConvert->getMovementPatternArrayIterator(); it!= theBehaviorToConvert->getMovementPatternArrayIteratorEnd();it++) {
            displayMovementStructure theSingleStructure = displayMovementStructure();
            theSingleStructure.mvAtkCurrentType = MOVEMENT_TYPE;
            if (it->getInstanceChessPiecePatternType() == POINT_TYPE) {
                int tmpmvAtkPositionX = in_X+((ChessPiecePointPattern*)(&*it))->getXPoint();
                int tmpmvAtkPositionY = in_Y+((ChessPiecePointPattern*)(&*it))->getYPoint();
                //cannot move beyond the gamefield, skipping
                if (tmpmvAtkPositionX < 0 || tmpmvAtkPositionY < 0 || tmpmvAtkPositionX>=this->internalBoardSize || tmpmvAtkPositionY>=this->internalBoardSize) {continue;}
                //check other figurines on that position
                ChessPieceOnField* collisionFigurine = this->findByPosition(tmpmvAtkPositionX, tmpmvAtkPositionY);
                if (collisionFigurine == nullptr) { //no chess piece on that position, OK to move
                    //there may be required additional checks for KING chess piece: king cannot move on a position under attack

                    theSingleStructure.mvAtkPositionX = tmpmvAtkPositionX;
                    theSingleStructure.mvAtkPositionY = tmpmvAtkPositionY;

                } else { //there is another chess piece there...
                    if (instChessPiece->getCurrentSideType() != collisionFigurine->getCurrentSideType()) {
                        theSingleStructure.mvAtkCurrentType = ATTACK_TYPE;
                        theSingleStructure.mvAtkPositionX = tmpmvAtkPositionX;
                        theSingleStructure.mvAtkPositionY = tmpmvAtkPositionY;
                    } else { //do not do anything, it's an ally.
                        continue;
                    }
                }
                currentStructureListMovements.push_back(theSingleStructure);
            }
        }
        movementInfoModel->setAllMvAtkStructures(currentStructureListMovements);
    }
}

ChessPieceOnField *ChessFieldPieces::findByPosition(int in_X, int in_Y) {
    for(std::vector<ChessPieceOnField>::iterator it = allChessPiecesDisplayed.begin(); it != allChessPiecesDisplayed.end(); ++it) {
        if ( ((it)->getCurrentXonField() != in_X) || ((it)->getCurrentYonField() != in_Y) ) {
            continue;
        }
        //get ptr from iterator: https://stackoverflow.com/a/2756254
        return (&*it);
    }
    return nullptr;
}

ChessPieceOnField *ChessFieldPieces::findByPosition(int in_X, int in_Y, unsigned int *inout_indexptr) {
    if (inout_indexptr == nullptr) {return nullptr;} unsigned int currentIndex = 0;
    for(std::vector<ChessPieceOnField>::iterator it = allChessPiecesDisplayed.begin(); it != allChessPiecesDisplayed.end(); ++it) {
        if ( ((it)->getCurrentXonField() != in_X) || ((it)->getCurrentYonField() != in_Y) ) {
            currentIndex++;
            continue;
        }
        //get ptr from iterator: https://stackoverflow.com/a/2756254
        *inout_indexptr = currentIndex;
        return (&*it);
    }
    return nullptr;
}

void ChessFieldPieces::activateClearModelData() {
    if (this->movementInfoModel == nullptr) {
        return;
    }
    this->movementInfoModel->clearModelData();
}

int ChessFieldPieces::move(int fromX, int fromY, int toX, int toY) {
    if (this->movementInfoModel == nullptr) {
        return 2;
    }
    //find out if toX, toY are in movementInfoModel
    for(std::vector<displayMovementStructure>::iterator it=this->movementInfoModel->getMvAtkStructuresIteratorBegin(); it!=this->movementInfoModel->getMvAtkStructuresIteratorEnd(); it++) {
        if( (it->mvAtkPositionX != toX) || (it->mvAtkPositionY != toY) ) {
            //not found by now, continue lookup
            continue;
        }
        //FOUND, perform movement
        unsigned int initialChessPieceIndex; unsigned int rivalChessPieceIndex;
        ChessPieceOnField* initialChessPiece = this->findByPosition(fromX, fromY, &initialChessPieceIndex);
        ChessPieceOnField* possibleRivalChessPiece = this->findByPosition(toX, toY, &rivalChessPieceIndex);
        if (initialChessPiece != nullptr) {
            if (possibleRivalChessPiece != nullptr) { //there is something!
                if (possibleRivalChessPiece->getCurrentSideType()!=initialChessPiece->getCurrentSideType()) {
                    //beginResetModel and endResetModel cause ReferenceError: mylogic is not defined in qml file
                    //beginResetModel();
                    //move the piece
                    initialChessPiece->setCurrentXonField(toX);
                    initialChessPiece->setCurrentYonField(toY);
                    //this->movementInfoModel->removeItemByIndex(rivalChessPieceIndex);

                    this->beginRemoveRows(QModelIndex(),rivalChessPieceIndex,rivalChessPieceIndex);
                    this->allChessPiecesDisplayed.erase(this->allChessPiecesDisplayed.begin()+rivalChessPieceIndex);
                    this->endRemoveRows();
                    //update index after removing item
                    //findByPosition(toX, toY, &initialChessPieceIndex);
                    QModelIndex topLeft = createIndex(initialChessPieceIndex, 0);
                    QModelIndex bottomRight = createIndex(initialChessPieceIndex, 0);
                    emit dataChanged(topLeft, bottomRight);
                    initialChessPiece->getInstBehaviorChessPiece()->performActionsAfterMovement( (void*)(initialChessPiece->getMovementPerformed() ) );
                    this->increaseCurrentMove();
                    //endResetModel();
                } else {
                    return 2;
                }
            } else {
                //nothing here, move the piece
                initialChessPiece->setCurrentXonField(toX);
                initialChessPiece->setCurrentYonField(toY);
                QModelIndex topLeft = createIndex(initialChessPieceIndex, 0);
                QModelIndex bottomRight = createIndex(initialChessPieceIndex, 0);
                emit dataChanged(topLeft, bottomRight);
                initialChessPiece->getInstBehaviorChessPiece()->performActionsAfterMovement( (void*)(initialChessPiece->getMovementPerformed() ) );
                this->increaseCurrentMove();
                //update model
            }
        } else { //there is no figurine at FROM location!
            return 2;
        }
        return 1;
    }
    //movement not possible
    return 0;
}

void ChessFieldPieces::fillGameField() {
    unsigned numberOfItemsToInsert = 3;
    this->beginInsertRows(QModelIndex(),0,numberOfItemsToInsert-1);
    ChessPieceOnField whiteKing = ChessPieceOnField(globalBehaviorCollection.at(KING_TYPE) , "/images/white_king.svg");
    whiteKing.setCurrentXonField(1); whiteKing.setCurrentYonField(2); whiteKing.setCurrentSideType(WHITE);
    this->appendPieceOnField(whiteKing);

    ChessPieceOnField blackPawn = ChessPieceOnField(globalBehaviorCollection.at(BLACK_PAWN_TYPE), "/images/black_pawn.svg");
    blackPawn.setCurrentXonField(2); blackPawn.setCurrentYonField(2); blackPawn.setCurrentSideType(BLACK);
    this->appendPieceOnField(blackPawn);

    ChessPieceOnField blackKing = ChessPieceOnField(globalBehaviorCollection.at(KING_TYPE), "/images/black_king.svg");
    blackKing.setCurrentXonField(3); blackKing.setCurrentYonField(3); blackKing.setCurrentSideType(BLACK);
    this->appendPieceOnField(blackKing);
    this->endInsertRows();
}

void ChessFieldPieces::clearGameField() {
    unsigned numberOfItemsToRemove = this->allChessPiecesDisplayed.size();
    this->beginRemoveRows(QModelIndex(),0,numberOfItemsToRemove-1);
    this->allChessPiecesDisplayed.clear();
    this->endRemoveRows();
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

ChessPieceMetadataBehavior* ChessPieceOnField::getInstBehaviorChessPiece() const
{
    return instBehaviorChessPiece;
}

void ChessPieceOnField::setMovementPerformed(const uint8_t &value) {
    movementPerformed = value;
}

uint8_t *ChessPieceOnField::getMovementPerformed() {
    return &movementPerformed;
}

ChessPieceSideTypes ChessPieceOnField::getCurrentSideType() const
{
    return currentSideType;
}

void ChessPieceOnField::setCurrentSideType(const ChessPieceSideTypes &value)
{
    currentSideType = value;
}

ChessPieceOnField::ChessPieceOnField(ChessPieceMetadataBehavior* in_Behavior, std::__cxx11::string in_pathToImage)
{
    this->pathToImage = in_pathToImage;
    this->instBehaviorChessPiece = in_Behavior;
    movementPerformed = 0;
}

std::string ChessPieceOnField::getPathToImage() const
{
    return pathToImage;
}

void ChessPieceOnField::setPathToImage(const std::string &value)
{
    pathToImage = value;
}
