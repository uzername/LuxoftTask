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

void ChessFieldPieces::setFieldPieceBitMapVariable(ChessPieceOnField *in_piece) {
    //check piece type
    uint8_t* bitmapToPlace = in_piece->getMovementPerformed();
    ChessPieceBehaviorTypes theBehaviorIdentifier = in_piece->getInstBehaviorChessPiece()->getCurrentBehaviorType();
    //uint8_t sideOfPiece = in_piece->getCurrentSideType();
    uint8_t sideOfPiece = 0;
    sideOfPiece = sideOfPiece << 3;
    if ((*bitmapToPlace & BM_movementperformed) == 1) { return; }
    switch (theBehaviorIdentifier) {
        case (WHITE_PAWN_TYPE): { //move from south to north
            if ((in_piece->getCurrentYonField()-1 >=0)&&(this->findByPosition(in_piece->getCurrentXonField(), in_piece->getCurrentYonField()-1 ))) { //chess piece to south?
                *bitmapToPlace = *bitmapToPlace | BM_gotpiecenorthorsouth;
            } else {
                *bitmapToPlace = *bitmapToPlace & ~BM_gotpiecenorthorsouth;
            }
            if ((in_piece->getCurrentYonField()-2 >=0)&&(this->findByPosition(in_piece->getCurrentXonField(), in_piece->getCurrentYonField()-2 )) ) { //chess piece to farsouth?
                *bitmapToPlace = *bitmapToPlace | BM_gotpieceFarNorthorFarSouth;
            } else {
                *bitmapToPlace = *bitmapToPlace & ~BM_gotpieceFarNorthorFarSouth;
            }
            *bitmapToPlace = *bitmapToPlace | sideOfPiece;
            break;
        }
        case (BLACK_PAWN_TYPE): { //moves from north to south
            if ( (in_piece->getCurrentYonField()+1 < this->boardSize() )&&(this->findByPosition(in_piece->getCurrentXonField(), in_piece->getCurrentYonField()+1 )) ) { //chess piece to north?
                *bitmapToPlace = *bitmapToPlace | BM_gotpiecenorthorsouth;
            } else {
                *bitmapToPlace = *bitmapToPlace & ~BM_gotpiecenorthorsouth;
            }
            if ((in_piece->getCurrentYonField()+2 < this->boardSize() )&&(this->findByPosition(in_piece->getCurrentXonField(), in_piece->getCurrentYonField()+2 ))) { //chess piece to farnorth?
                *bitmapToPlace = *bitmapToPlace | BM_gotpieceFarNorthorFarSouth;
            } else {
                *bitmapToPlace = *bitmapToPlace & ~BM_gotpieceFarNorthorFarSouth;
            }
            *bitmapToPlace = *bitmapToPlace | sideOfPiece;
            break;
        default: {
            break;
            }
        }
    }
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
    if (instChessPiece == nullptr) { //it means a critical failure!
        return;
    }
    if (this->getCurrentMove()!=instChessPiece->getCurrentSideType()) {
        return;
    }
    if (instChessPiece!=nullptr) {
        ChessPieceMetadataBehavior* theBehaviorToConvert = instChessPiece->getInstBehaviorChessPiece();
        this->setFieldPieceBitMapVariable(instChessPiece);
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
                    //better to resolve attack in a separate similar cycle. Skip this move by now
                    continue;
                }
                currentStructureListMovements.push_back(theSingleStructure);
            }
        }
        for (std::vector<ChessPiecePointPattern>::iterator it = theBehaviorToConvert->getAttackPatternArrayIterator(); it!=theBehaviorToConvert->getAttackPatternArrayIteratorEnd(); it++) {
            displayMovementStructure theSingleStructure = displayMovementStructure();
            theSingleStructure.mvAtkCurrentType = ATTACK_TYPE;
            if (it->getInstanceChessPiecePatternType() == POINT_TYPE) {
                int tmpmvAtkPositionX = in_X+((ChessPiecePointPattern*)(&*it))->getXPoint();
                int tmpmvAtkPositionY = in_Y+((ChessPiecePointPattern*)(&*it))->getYPoint();
                //cannot move beyond the gamefield, skipping
                if (tmpmvAtkPositionX < 0 || tmpmvAtkPositionY < 0 || tmpmvAtkPositionX>=this->internalBoardSize || tmpmvAtkPositionY>=this->internalBoardSize) {continue;}
                //check other figurines on that position
                ChessPieceOnField* collisionFigurine = this->findByPosition(tmpmvAtkPositionX, tmpmvAtkPositionY);
                if (collisionFigurine == nullptr) { //no chess piece on that position, not moving
                    continue;
                } else {
                    if (instChessPiece->getCurrentSideType() != collisionFigurine->getCurrentSideType()) {
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
    if ((fromX == toX)&&(fromY==toY)) {
        return 0;
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
                if ((possibleRivalChessPiece->getCurrentSideType()!=initialChessPiece->getCurrentSideType())) {
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
    unsigned numberOfItemsToInsert = 4;
    this->beginInsertRows(QModelIndex(),0,numberOfItemsToInsert-1);
    ChessPieceOnField whiteKing = ChessPieceOnField(globalBehaviorCollection.at(KING_TYPE) , "/images/white_king.svg");
    whiteKing.setCurrentXonField(4); whiteKing.setCurrentYonField(7); whiteKing.setCurrentSideType(WHITE);
    this->appendPieceOnField(whiteKing);

    ChessPieceOnField whitePawn = ChessPieceOnField(globalBehaviorCollection.at(WHITE_PAWN_TYPE), "/images/white_pawn.svg");
    whitePawn.setCurrentXonField(0); whitePawn.setCurrentYonField(6); whitePawn.setCurrentSideType(WHITE);
    this->appendPieceOnField(whitePawn);

    ChessPieceOnField blackPawn = ChessPieceOnField(globalBehaviorCollection.at(BLACK_PAWN_TYPE), "/images/black_pawn.svg");
    blackPawn.setCurrentXonField(0); blackPawn.setCurrentYonField(1); blackPawn.setCurrentSideType(BLACK);
    this->appendPieceOnField(blackPawn);

    ChessPieceOnField blackKing = ChessPieceOnField(globalBehaviorCollection.at(KING_TYPE), "/images/black_king.svg");
    blackKing.setCurrentXonField(4); blackKing.setCurrentYonField(3); blackKing.setCurrentSideType(BLACK);
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
