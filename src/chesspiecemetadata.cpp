#include "chesspiecemetadata.h"

ChessPieceBehaviorTypes ChessPieceMetadataBehavior::getCurrentBehaviorType() const
{
    return currentBehaviorType;
}

void ChessPieceMetadataBehavior::setCurrentBehaviorType(const ChessPieceBehaviorTypes &value)  {
    currentBehaviorType = value;
}

std::vector<ChessPiecePointPattern>::iterator ChessPieceMetadataBehavior::getMovementPatternArrayIterator() {
    return this->currentPointMovementPattern.begin();
}

std::vector<ChessPiecePointPattern>::iterator ChessPieceMetadataBehavior::getMovementPatternArrayIteratorEnd()
{
    return this->currentPointMovementPattern.end();
}

std::vector<ChessPiecePointPattern>::iterator ChessPieceMetadataBehavior::getAttackPatternArrayIterator() {
    return this->currentPointAttackPattern.begin();
}

std::vector<ChessPiecePointPattern>::iterator ChessPieceMetadataBehavior::getAttackPatternArrayIteratorEnd() {
    return this->currentPointAttackPattern.end();
}

void ChessPieceMetadataBehavior::performActionsAfterMovement(void *ud)
{

}

void ChessPieceMetadataBehavior::performActionsBeforeMovement(void *ud)
{

}

ChessPieceMetadataBehavior::ChessPieceMetadataBehavior() {
    this->currentPointMovementPattern = std::vector<ChessPiecePointPattern>();
    this->currentPointAttackPattern = std::vector<ChessPiecePointPattern>();
}

ChessPiecePatternTypes ChessPiecePattern::getInstanceChessPiecePatternType() const
{
    return instanceChessPiecePatternType;
}

void ChessPiecePattern::setInstanceChessPiecePatternType(const ChessPiecePatternTypes &value)
{
    instanceChessPiecePatternType = value;
}

ChessPiecePatternVectorTypes ChessPieceVectorPattern::getTheDirectionType() const
{
    return theDirectionType;
}

ChessPieceVectorPattern::ChessPieceVectorPattern(ChessPiecePatternVectorTypes in_DirectionType)
{
    this->instanceChessPiecePatternType = VECTOR_TYPE;
    this->theDirectionType = in_DirectionType;
}

int ChessPiecePointPattern::getYPoint() const
{
    return YPoint;
}

ChessPiecePointPattern::ChessPiecePointPattern(int in_XPoint, int in_YPoint)
{
    this->instanceChessPiecePatternType = POINT_TYPE;
    this->XPoint = in_XPoint;
    this->YPoint = in_YPoint;
}

int ChessPiecePointPattern::getXPoint() const {
    return XPoint;
}

KingBehavior::KingBehavior()
    :ChessPieceMetadataBehavior() {
    this->currentBehaviorType = KING_TYPE;
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(0,1));
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(0,-1));
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(1,0));
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(-1,0));
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(1,1));
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(-1,1));
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(1,-1));
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(-1,-1));

}

BlackPawnBehavior::BlackPawnBehavior()
    :ChessPieceMetadataBehavior() {
    this->currentBehaviorType = BLACK_PAWN_TYPE;
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(0,1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(1,1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(1,-1));

}

void BlackPawnBehavior::performActionsAfterMovement(void *ud)
{
    *((uint8_t*)(ud)) = 1;
}

void BlackPawnBehavior::performActionsBeforeMovement(void *ud)
{
    //add additional move if chess piece has not moved yet
    if (*((uint8_t*)(ud)) == 0) {
        this->currentPointMovementPattern.push_back(ChessPiecePointPattern(0,2));
    } else {
        for (unsigned i=0; i<currentPointMovementPattern.size(); i++ ){
            if (( ((ChessPiecePointPattern)currentPointMovementPattern[i]).getXPoint() == 0 )&&(((ChessPiecePointPattern)currentPointMovementPattern[i]).getYPoint() == 2)) {
                currentPointMovementPattern.erase(currentPointMovementPattern.begin()+i);
            }
        }
    }
}

//========
/*
KingBehavior* instKingBehavior;
BlackPawnBehavior* instBlackPawnBehavior;
*/
std::unordered_map<int, ChessPieceMetadataBehavior*> globalBehaviorCollection;
void initBehaviors() {
    /*
    instKingBehavior = new KingBehavior();
    instBlackPawnBehavior = new BlackPawnBehavior();
    */
    globalBehaviorCollection = std::unordered_map<int, ChessPieceMetadataBehavior*>();
    globalBehaviorCollection.insert(std::make_pair<int, ChessPieceMetadataBehavior*>(KING_TYPE, new KingBehavior()) );
    globalBehaviorCollection.insert(std::make_pair<int, ChessPieceMetadataBehavior*>(BLACK_PAWN_TYPE, new BlackPawnBehavior()) );
}

void deinitBehaviors() {
    /*
    delete instKingBehavior;
    delete instBlackPawnBehavior;
    */
    for (std::unordered_map<int,ChessPieceMetadataBehavior*>::iterator it2=globalBehaviorCollection.begin(); it2!=globalBehaviorCollection.end(); it2++) {
        delete (*it2).second;
    }
}
//=======

