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

void ChessFieldPieces::signHistoryHandlerPact(HistoryHandlerData *in_historyMegaObject) {
    historyMegaObject = in_historyMegaObject;
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
        //===POINT MOVEMENT AND ATTACK PATTERNS===
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
        //===VECTOR MOVEMENT AND ATTACK PATTERNS===
        for (std::vector<ChessPieceVectorPattern>::iterator it = theBehaviorToConvert->getVectorMovementPatternArrayIterator(); it!=theBehaviorToConvert->getVectorMovementPatternArrayIteratorEnd(); it++) {
            displayMovementStructure theSingleStructure = displayMovementStructure();
            theSingleStructure.mvAtkCurrentType = MOVEMENT_TYPE;
            ChessPiecePatternVectorTypes currentTypeOfVector = ((it))->getTheDirectionType();
            int dx; int dy;
            switch (currentTypeOfVector) {
                case EAST: {
                    dx = 1; dy=0; break;
                }
                case WEST: {
                    dx=-1; dy=0; break;
                }
                case NORTH: {
                    dx=0; dy=1; break;
                }
                case SOUTH: {
                    dx=0; dy=-1; break;
                }
                case NORTHEAST: {
                    dx=1; dy=1; break;
                }
                case NORTHWEST: {
                    dx=-1; dy=1; break;
                }
                case SOUTHEAST: {
                    dx=1; dy=-1; break;
                }
                case SOUTHWEST: {
                    dx=-1; dy=-1; break;
                }
                default: {
                    return;
                    break;
                }
            }
                int tmpmvAtkPositionX = in_X+dx; int tmpmvAtkPositionY = in_Y+dy;
                ChessPieceOnField* collisionFigurine = this->findByPosition(tmpmvAtkPositionX, tmpmvAtkPositionY);
                while ((collisionFigurine==nullptr)&&(tmpmvAtkPositionX>=0)&&(tmpmvAtkPositionY>=0)&&(tmpmvAtkPositionX<=this->boardSize()-1)&&(tmpmvAtkPositionY<=this->boardSize()-1)) {
                    theSingleStructure.mvAtkPositionX = tmpmvAtkPositionX;
                    theSingleStructure.mvAtkPositionY = tmpmvAtkPositionY;
                    theSingleStructure.mvAtkCurrentType = MOVEMENT_TYPE;
                    currentStructureListMovements.push_back(theSingleStructure);
                    tmpmvAtkPositionX+=dx; tmpmvAtkPositionY+=dy;
                    collisionFigurine = this->findByPosition(tmpmvAtkPositionX, tmpmvAtkPositionY);
                }

        }

        for (std::vector<ChessPieceVectorPattern>::iterator it = theBehaviorToConvert->getVectorAttackPatternArrayIterator(); it!=theBehaviorToConvert->getVectorAttackPatternArrayIteratorEnd(); it++) {
            displayMovementStructure theSingleStructure = displayMovementStructure();
            theSingleStructure.mvAtkCurrentType = ATTACK_TYPE;
            ChessPiecePatternVectorTypes currentTypeOfVector = ((it))->getTheDirectionType();
            int dx; int dy;
            switch (currentTypeOfVector) {
                case EAST: {
                    dx = 1; dy=0; break;
                }
                case WEST: {
                    dx=-1; dy=0; break;
                }
                case NORTH: {
                    dx=0; dy=1; break;
                }
                case SOUTH: {
                    dx=0; dy=-1; break;
                }
                case NORTHEAST: {
                    dx=1; dy=1; break;
                }
                case NORTHWEST: {
                    dx=-1; dy=1; break;
                }
                case SOUTHEAST: {
                    dx=1; dy=-1; break;
                }
                case SOUTHWEST: {
                    dx=-1; dy=-1; break;
                }
                default: {
                    return;
                    break;
                }
            }
                int tmpmvAtkPositionX = in_X; int tmpmvAtkPositionY = in_Y;
                ChessPieceOnField* collisionFigurine = this->findByPosition(tmpmvAtkPositionX, tmpmvAtkPositionY);
                do {
                    tmpmvAtkPositionX+=dx; tmpmvAtkPositionY+=dy;
                    collisionFigurine = this->findByPosition(tmpmvAtkPositionX, tmpmvAtkPositionY);
                } while ((collisionFigurine==nullptr)&&(tmpmvAtkPositionX>=0)&&(tmpmvAtkPositionY>=0)&&(tmpmvAtkPositionX<=this->boardSize()-1)&&(tmpmvAtkPositionY<=this->boardSize()-1));
                if (collisionFigurine!=nullptr) {
                    if (collisionFigurine->getCurrentSideType()!=instChessPiece->getCurrentSideType()) {
                    theSingleStructure.mvAtkPositionX = tmpmvAtkPositionX;
                    theSingleStructure.mvAtkPositionY = tmpmvAtkPositionY;
                    theSingleStructure.mvAtkCurrentType = ATTACK_TYPE;
                    currentStructureListMovements.push_back(theSingleStructure);
                    }
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
            History_SingleMovement movementInHistory;
            movementInHistory.capturePerformed = 0; movementInHistory.capturedUniqueIndex = 0;
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
                    movementInHistory.capturePerformed = 1;
                    movementInHistory.capturedUniqueIndex = possibleRivalChessPiece->getUniqueId();
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
            //the movement has been performed if we are here
            movementInHistory.uniqueIndexOfFigurine = initialChessPiece->getUniqueId();
            movementInHistory.startX = fromX;
            movementInHistory.startY = fromY;
            movementInHistory.endX = toX;
            movementInHistory.endY = toY;
            this->historyMegaObject->addSingleMovement(movementInHistory);
        } else { //there is no figurine at FROM location!
            return 2;
        }
        return 1;
    }
    //movement not possible
    return 0;
}

void ChessFieldPieces::fillGameField() {
            this->historyMegaObject->quickCleanupAllHistory();
    //precisely set this number to keep model in actual state!
    unsigned numberOfItemsToInsert = 18;
    this->beginInsertRows(QModelIndex(),0,numberOfItemsToInsert-1); uint16_t uniqueIdIndex=1;
    History_SingleInitialStateOfFigurine initialStateToBeAdded;
    try {
    ChessPieceOnField whitePawn = ChessPieceOnField(globalBehaviorCollection.at(WHITE_PAWN_TYPE), "/images/white_pawn.svg");
    whitePawn.setCurrentXonField(0); whitePawn.setCurrentYonField(6); whitePawn.setCurrentSideType(WHITE);
    whitePawn.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(whitePawn);
        initialStateToBeAdded.BehaviorType = WHITE_PAWN_TYPE; initialStateToBeAdded.SideType = WHITE;
        initialStateToBeAdded.PathToImage = "/images/white_pawn.svg";
        initialStateToBeAdded.XcoordOnField = 0; initialStateToBeAdded.YcoordOnField = 6;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
    ChessPieceOnField whiteKing = ChessPieceOnField(globalBehaviorCollection.at(KING_TYPE) , "/images/white_king.svg");
    whiteKing.setCurrentXonField(4); whiteKing.setCurrentYonField(7); whiteKing.setCurrentSideType(WHITE);
    whiteKing.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(whiteKing);
        initialStateToBeAdded.BehaviorType = KING_TYPE; initialStateToBeAdded.SideType = WHITE;
        initialStateToBeAdded.PathToImage = "/images/white_king.svg";
        initialStateToBeAdded.XcoordOnField = 4; initialStateToBeAdded.YcoordOnField = 7;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try{
    ChessPieceOnField whiteRook1 = ChessPieceOnField(globalBehaviorCollection.at(ROOK_TYPE), "/images/white_rook.svg");
    whiteRook1.setCurrentXonField(0); whiteRook1.setCurrentYonField(7); whiteRook1.setCurrentSideType(WHITE);
    whiteRook1.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(whiteRook1);
        initialStateToBeAdded.BehaviorType = ROOK_TYPE; initialStateToBeAdded.SideType = WHITE;
        initialStateToBeAdded.PathToImage = "/images/white_rook.svg";
        initialStateToBeAdded.XcoordOnField = 0; initialStateToBeAdded.YcoordOnField = 7;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
    ChessPieceOnField whiteRook2 = ChessPieceOnField(globalBehaviorCollection.at(ROOK_TYPE), "/images/white_rook.svg");
    whiteRook2.setCurrentXonField(7); whiteRook2.setCurrentYonField(7); whiteRook2.setCurrentSideType(WHITE);
    whiteRook2.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(whiteRook2);
        initialStateToBeAdded.BehaviorType = ROOK_TYPE; initialStateToBeAdded.SideType = WHITE;
        initialStateToBeAdded.PathToImage = "/images/white_rook.svg";
        initialStateToBeAdded.XcoordOnField = 7; initialStateToBeAdded.YcoordOnField = 7;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
    ChessPieceOnField whiteBishop1 = ChessPieceOnField(globalBehaviorCollection.at(BISHOP_TYPE), "/images/white_bishop.svg");
    whiteBishop1.setCurrentXonField(2); whiteBishop1.setCurrentYonField(7); whiteBishop1.setCurrentSideType(WHITE);
    whiteBishop1.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(whiteBishop1);
        initialStateToBeAdded.BehaviorType = BISHOP_TYPE; initialStateToBeAdded.SideType = WHITE;
        initialStateToBeAdded.PathToImage = "/images/white_bishop.svg";
        initialStateToBeAdded.XcoordOnField = 2; initialStateToBeAdded.YcoordOnField = 7;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
    ChessPieceOnField whiteBishop2 = ChessPieceOnField(globalBehaviorCollection.at(BISHOP_TYPE), "/images/white_bishop.svg");
    whiteBishop2.setCurrentXonField(5); whiteBishop2.setCurrentYonField(7); whiteBishop2.setCurrentSideType(WHITE);
    whiteBishop2.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(whiteBishop2);
        initialStateToBeAdded.BehaviorType = BISHOP_TYPE; initialStateToBeAdded.SideType = WHITE;
        initialStateToBeAdded.PathToImage = "/images/white_bishop.svg";
        initialStateToBeAdded.XcoordOnField = 5; initialStateToBeAdded.YcoordOnField = 7;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
        ChessPieceOnField whiteKnight1 = ChessPieceOnField(globalBehaviorCollection.at(KNIGHT_TYPE), "/images/white_knight.svg");
        whiteKnight1.setCurrentXonField(6); whiteKnight1.setCurrentYonField(7); whiteKnight1.setCurrentSideType(WHITE);
        whiteKnight1.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
        this->appendPieceOnField(whiteKnight1);
            initialStateToBeAdded.BehaviorType = KNIGHT_TYPE; initialStateToBeAdded.SideType = WHITE;
            initialStateToBeAdded.PathToImage = "/images/white_knight.svg";
            initialStateToBeAdded.XcoordOnField = 6; initialStateToBeAdded.YcoordOnField = 7;
            initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
            this->historyMegaObject->addInitialState(initialStateToBeAdded);

    } catch (std::out_of_range& e) {  }
    try {
        ChessPieceOnField whiteKnight2 = ChessPieceOnField(globalBehaviorCollection.at(KNIGHT_TYPE), "/images/white_knight.svg");
        whiteKnight2.setCurrentXonField(1); whiteKnight2.setCurrentYonField(7); whiteKnight2.setCurrentSideType(WHITE);
        this->appendPieceOnField(whiteKnight2);
            initialStateToBeAdded.BehaviorType = KNIGHT_TYPE; initialStateToBeAdded.SideType = WHITE;
            initialStateToBeAdded.PathToImage = "/images/white_knight.svg";
            initialStateToBeAdded.XcoordOnField = 1; initialStateToBeAdded.YcoordOnField = 7;
            initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
            this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
        ChessPieceOnField whiteQueen = ChessPieceOnField(globalBehaviorCollection.at(QUEEN_TYPE), "/images/white_queen.svg");
        whiteQueen.setCurrentXonField(3); whiteQueen.setCurrentYonField(7); whiteQueen.setCurrentSideType(WHITE);
        whiteQueen.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
        this->appendPieceOnField(whiteQueen);
            initialStateToBeAdded.BehaviorType = QUEEN_TYPE; initialStateToBeAdded.SideType = WHITE;
            initialStateToBeAdded.PathToImage = "/images/white_queen.svg";
            initialStateToBeAdded.XcoordOnField = 3; initialStateToBeAdded.YcoordOnField = 7;
            initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
            this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
    ChessPieceOnField blackPawn = ChessPieceOnField(globalBehaviorCollection.at(BLACK_PAWN_TYPE), "/images/black_pawn.svg");
    blackPawn.setCurrentXonField(0); blackPawn.setCurrentYonField(1); blackPawn.setCurrentSideType(BLACK);
    blackPawn.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(blackPawn);
        initialStateToBeAdded.BehaviorType = BLACK_PAWN_TYPE; initialStateToBeAdded.SideType = BLACK;
        initialStateToBeAdded.PathToImage = "/images/black_pawn.svg";
        initialStateToBeAdded.XcoordOnField = 0; initialStateToBeAdded.YcoordOnField = 1;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
    ChessPieceOnField blackKing = ChessPieceOnField(globalBehaviorCollection.at(KING_TYPE), "/images/black_king.svg");
    blackKing.setCurrentXonField(4); blackKing.setCurrentYonField(0); blackKing.setCurrentSideType(BLACK);
    blackKing.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(blackKing);
        initialStateToBeAdded.BehaviorType = KING_TYPE; initialStateToBeAdded.SideType = BLACK;
        initialStateToBeAdded.PathToImage = "/images/black_king.svg";
        initialStateToBeAdded.XcoordOnField = 4; initialStateToBeAdded.YcoordOnField = 0;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
    ChessPieceOnField blackRook1 = ChessPieceOnField(globalBehaviorCollection.at(ROOK_TYPE), "/images/black_rook.svg");
    blackRook1.setCurrentXonField(0); blackRook1.setCurrentYonField(0); blackRook1.setCurrentSideType(BLACK);
    blackRook1.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(blackRook1);
        initialStateToBeAdded.BehaviorType = ROOK_TYPE; initialStateToBeAdded.SideType = BLACK;
        initialStateToBeAdded.PathToImage = "/images/black_rook.svg";
        initialStateToBeAdded.XcoordOnField = 0; initialStateToBeAdded.YcoordOnField = 0;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
    ChessPieceOnField blackRook2 = ChessPieceOnField(globalBehaviorCollection.at(ROOK_TYPE), "/images/black_rook.svg");
    blackRook2.setCurrentXonField(7); blackRook2.setCurrentYonField(0); blackRook2.setCurrentSideType(BLACK);
    blackRook2.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(blackRook2);
        initialStateToBeAdded.BehaviorType = ROOK_TYPE; initialStateToBeAdded.SideType = BLACK;
        initialStateToBeAdded.PathToImage = "/images/black_rook.svg";
        initialStateToBeAdded.XcoordOnField = 7; initialStateToBeAdded.YcoordOnField = 0;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
    ChessPieceOnField blackBishop1 = ChessPieceOnField(globalBehaviorCollection.at(BISHOP_TYPE), "/images/black_bishop.svg");
    blackBishop1.setCurrentXonField(2); blackBishop1.setCurrentYonField(0); blackBishop1.setCurrentSideType(BLACK);
    blackBishop1.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(blackBishop1);
        initialStateToBeAdded.BehaviorType = BISHOP_TYPE; initialStateToBeAdded.SideType = BLACK;
        initialStateToBeAdded.PathToImage = "/images/black_bishop.svg";
        initialStateToBeAdded.XcoordOnField = 2; initialStateToBeAdded.YcoordOnField = 0;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
    ChessPieceOnField blackBishop2 = ChessPieceOnField(globalBehaviorCollection.at(BISHOP_TYPE), "/images/black_bishop.svg");
    blackBishop2.setCurrentXonField(5); blackBishop2.setCurrentYonField(0); blackBishop2.setCurrentSideType(BLACK);
    blackBishop2.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
    this->appendPieceOnField(blackBishop2);
        initialStateToBeAdded.BehaviorType = BISHOP_TYPE; initialStateToBeAdded.SideType = BLACK;
        initialStateToBeAdded.PathToImage = "/images/black_bishop.svg";
        initialStateToBeAdded.XcoordOnField = 5; initialStateToBeAdded.YcoordOnField = 0;
        initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
        this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
        ChessPieceOnField blackKnight1 = ChessPieceOnField(globalBehaviorCollection.at(KNIGHT_TYPE), "/images/black_knight.svg");
        blackKnight1.setCurrentXonField(6); blackKnight1.setCurrentYonField(0); blackKnight1.setCurrentSideType(BLACK);
        blackKnight1.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
        this->appendPieceOnField(blackKnight1);
            initialStateToBeAdded.BehaviorType = KNIGHT_TYPE; initialStateToBeAdded.SideType = BLACK;
            initialStateToBeAdded.PathToImage = "/images/black_knight.svg";
            initialStateToBeAdded.XcoordOnField = 6; initialStateToBeAdded.YcoordOnField = 0;
            initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
            this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
        ChessPieceOnField blackKnight2 = ChessPieceOnField(globalBehaviorCollection.at(KNIGHT_TYPE), "/images/black_knight.svg");
        blackKnight2.setCurrentXonField(1); blackKnight2.setCurrentYonField(0); blackKnight2.setCurrentSideType(BLACK);
        blackKnight2.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
        this->appendPieceOnField(blackKnight2);
            initialStateToBeAdded.BehaviorType = KNIGHT_TYPE; initialStateToBeAdded.SideType = BLACK;
            initialStateToBeAdded.PathToImage = "/images/black_knight.svg";
            initialStateToBeAdded.XcoordOnField = 1; initialStateToBeAdded.YcoordOnField = 0;
            initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
            this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    try {
        ChessPieceOnField blackQueen = ChessPieceOnField(globalBehaviorCollection.at(QUEEN_TYPE), "/images/black_queen.svg");
        blackQueen.setCurrentXonField(3); blackQueen.setCurrentYonField(0); blackQueen.setCurrentSideType(BLACK);
        blackQueen.setUniqueId(uniqueIdIndex); uniqueIdIndex++;
        this->appendPieceOnField(blackQueen);
            initialStateToBeAdded.BehaviorType = QUEEN_TYPE; initialStateToBeAdded.SideType = BLACK;
            initialStateToBeAdded.PathToImage = "/images/black_queen.svg";
            initialStateToBeAdded.XcoordOnField = 3; initialStateToBeAdded.YcoordOnField = 0;
            initialStateToBeAdded.uniqueIndexOfFigurine = uniqueIdIndex-1;
            this->historyMegaObject->addInitialState(initialStateToBeAdded);
    } catch (std::out_of_range& e) {  }
    this->endInsertRows();
}

void ChessFieldPieces::clearGameField() {
        this->historyMegaObject->quickCleanupAllHistory();
    unsigned numberOfItemsToRemove = this->allChessPiecesDisplayed.size();
    if (numberOfItemsToRemove == 0) return;
    this->beginRemoveRows(QModelIndex(),0,numberOfItemsToRemove-1);
    this->allChessPiecesDisplayed.clear();
    this->endRemoveRows();
}

void ChessFieldPieces::fillGameFieldFromInitialHistory() {
    if (this->historyMegaObject == nullptr) {
        return;
    }
    unsigned numberOfItemsToInsert = this->historyMegaObject->getSizeInitialStates();
    this->beginInsertRows(QModelIndex(),0,numberOfItemsToInsert-1);
    for (std::vector<History_SingleInitialStateOfFigurine>::iterator itr = this->historyMegaObject->getStatesVectorIteratorBegin(); itr!=this->historyMegaObject->getStatesVectorIteratorEnd(); itr++) {
            ChessPieceOnField PieceTobeAdded = ChessPieceOnField(globalBehaviorCollection.at(itr->BehaviorType), itr->PathToImage);
            PieceTobeAdded.setCurrentXonField(itr->XcoordOnField); PieceTobeAdded.setCurrentYonField(itr->YcoordOnField);
            PieceTobeAdded.setCurrentSideType(itr->SideType); PieceTobeAdded.setUniqueId(itr->uniqueIndexOfFigurine);
            this->appendPieceOnField(PieceTobeAdded);
    }
    this->endInsertRows();
}
void ChessFieldPieces::appendPieceOnField(ChessPieceOnField pieceToAppend) {
    this->allChessPiecesDisplayed.push_back(pieceToAppend);
}
//=============================================
ChessIntegerCoordType ChessPieceOnField::getCurrentXonField() const
{
    return currentXonField;
}

void ChessPieceOnField::setCurrentXonField(const ChessIntegerCoordType &value) {
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

ChessPieceSideTypes ChessPieceOnField::getCurrentSideType() const {
    return currentSideType;
}

void ChessPieceOnField::setCurrentSideType(const ChessPieceSideTypes &value) {
    currentSideType = value;
}

uint16_t ChessPieceOnField::getUniqueId() const {
    return uniqueId;
}

void ChessPieceOnField::setUniqueId(const uint16_t &value) {
    uniqueId = value;
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
