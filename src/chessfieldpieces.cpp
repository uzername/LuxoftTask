#include "chessfieldpieces.h"

ChessFieldPieces::ChessFieldPieces(ChessIntegerCoordType in_boardSize, QObject *parent)
         : QAbstractListModel(parent)
{
    this->allChessPiecesDisplayed = std::vector<ChessPieceOnField>();
    this->internalBoardSize = in_boardSize;
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
                if (tmpmvAtkPositionX < 0 || tmpmvAtkPositionY < 0) {continue;}
                theSingleStructure.mvAtkPositionX = tmpmvAtkPositionX;
                theSingleStructure.mvAtkPositionY = tmpmvAtkPositionY;
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

void ChessFieldPieces::activateClearModelData() {
    if (this->movementInfoModel == nullptr) {
        return;
    }
    this->movementInfoModel->clearModelData();
}

uint8_t ChessFieldPieces::move(int fromX, int fromY, int toX, int toY)
{

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
