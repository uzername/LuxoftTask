#include "chesspiecemetadata.h"
#include "moredefs.h"

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

std::vector<ChessPieceVectorPattern>::iterator ChessPieceMetadataBehavior::getVectorMovementPatternArrayIterator() {
    return this->currentVectorMovementPattern.begin();
}

std::vector<ChessPieceVectorPattern>::iterator ChessPieceMetadataBehavior::getVectorMovementPatternArrayIteratorEnd() {
    return this->currentVectorMovementPattern.end();
}

std::vector<ChessPieceVectorPattern>::iterator ChessPieceMetadataBehavior::getVectorAttackPatternArrayIterator() {
    return this->currentVectorAttackPattern.begin();
}

std::vector<ChessPieceVectorPattern>::iterator ChessPieceMetadataBehavior::getVectorAttackPatternArrayIteratorEnd() {
    return this->currentVectorAttackPattern.end();
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

    this->currentVectorAttackPattern = std::vector<ChessPieceVectorPattern>();
    this->currentVectorMovementPattern = std::vector<ChessPieceVectorPattern>();
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

    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(0,1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(0,-1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(1,0));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(-1,0));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(1,1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(-1,1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(1,-1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(-1,-1));

}
WhitePawnBehavior::WhitePawnBehavior()
    :ChessPieceMetadataBehavior() {
    this->currentBehaviorType = WHITE_PAWN_TYPE;
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(0,-1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(-1,1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(-1,-1));
}
BlackPawnBehavior::BlackPawnBehavior()
    :ChessPieceMetadataBehavior() {
    this->currentBehaviorType = BLACK_PAWN_TYPE;
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(0,1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(1,1));
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(1,-1));

}
RookBehavior::RookBehavior()
    :ChessPieceMetadataBehavior() {
    this->currentBehaviorType = ROOK_TYPE;
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(SOUTH));
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(NORTH) );
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(EAST) );
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(WEST) );

    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(SOUTH) );
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(NORTH) );
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(EAST) );
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(WEST) );
}

BishopBehavior::BishopBehavior()
    :ChessPieceMetadataBehavior() {
    this->currentBehaviorType = BISHOP_TYPE;
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(NORTHEAST));
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(NORTHWEST));
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(SOUTHWEST));
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(SOUTHEAST));

    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(NORTHEAST));
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(NORTHWEST));
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(SOUTHWEST));
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(SOUTHEAST));
}

KnightBehavior::KnightBehavior()
 :ChessPieceMetadataBehavior() {
    this->currentBehaviorType = KNIGHT_TYPE;
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(1,-2)  );
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(-1,-2) );
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(1,2)   );
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(-1,2)  );
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(-2,1)  );
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(-2,-1) );
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(2,1)   );
    this->currentPointMovementPattern.push_back(ChessPiecePointPattern(2,-1)  );

    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(1,-2)  );
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(-1,-2) );
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(1,2)   );
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(-1,2)  );
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(-2,1)  );
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(-2,-1) );
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(2,1)   );
    this->currentPointAttackPattern.push_back(ChessPiecePointPattern(2,-1)  );
}
QueenBehavior::QueenBehavior()
    :ChessPieceMetadataBehavior() {
    this->currentBehaviorType = QUEEN_TYPE;
    //bishop
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(NORTHEAST));
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(NORTHWEST));
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(SOUTHWEST));
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(SOUTHEAST));

    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(NORTHEAST));
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(NORTHWEST));
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(SOUTHWEST));
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(SOUTHEAST));
    //rook
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(SOUTH));
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(NORTH) );
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(EAST) );
    this->currentVectorMovementPattern.push_back(ChessPieceVectorPattern(WEST) );

    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(SOUTH) );
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(NORTH) );
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(EAST) );
    this->currentVectorAttackPattern.push_back(ChessPieceVectorPattern(WEST) );
}
void BlackPawnBehavior::performActionsAfterMovement(void *ud)
{
    *((uint8_t*)(ud)) = 1;
}

void BlackPawnBehavior::performActionsBeforeMovement(void *ud)
{
    uint8_t internalMovementPerformed = *((uint8_t*)(ud));
    //add additional move if chess piece has not moved yet
    if ( ( (internalMovementPerformed & BM_movementperformed) == 0 ) &&
    (!(( (internalMovementPerformed & BM_gotpiecenorthorsouth) != 0 )||( (internalMovementPerformed & BM_gotpieceFarNorthorFarSouth) != 0))) )
    {
        //do not add move if there is a chess piece

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
    globalBehaviorCollection.insert(std::make_pair<int, ChessPieceMetadataBehavior*>(WHITE_PAWN_TYPE, new WhitePawnBehavior()) );
    globalBehaviorCollection.insert(std::make_pair<int, ChessPieceMetadataBehavior*>(ROOK_TYPE, new RookBehavior()) );
    globalBehaviorCollection.insert(std::make_pair<int, ChessPieceMetadataBehavior*>(BISHOP_TYPE, new BishopBehavior()) );
    globalBehaviorCollection.insert(std::make_pair<int, ChessPieceMetadataBehavior*>(KNIGHT_TYPE, new KnightBehavior()) );
    globalBehaviorCollection.insert(std::make_pair<int, ChessPieceMetadataBehavior*>(QUEEN_TYPE, new QueenBehavior()) );
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




void WhitePawnBehavior::performActionsAfterMovement(void *ud)
{
    *((uint8_t*)(ud)) = 1;
}

void WhitePawnBehavior::performActionsBeforeMovement(void *ud)
{
    uint8_t internalMovementPerformed = *((uint8_t*)(ud));
    //add additional move if chess piece has not moved yet
    if ( ( (internalMovementPerformed & BM_movementperformed) == 0 ) &&
    (!(( (internalMovementPerformed & BM_gotpiecenorthorsouth) != 0 )||( (internalMovementPerformed & BM_gotpieceFarNorthorFarSouth) != 0))) )
    {
        //do not add move if there is a chess piece

           this->currentPointMovementPattern.push_back(ChessPiecePointPattern(0,-2));
    } else {
        for (unsigned i=0; i<currentPointMovementPattern.size(); i++ ){
            if (( ((ChessPiecePointPattern)currentPointMovementPattern[i]).getXPoint() == 0 )&&(((ChessPiecePointPattern)currentPointMovementPattern[i]).getYPoint() == -2)) {
                currentPointMovementPattern.erase(currentPointMovementPattern.begin()+i);
            }
        }
    }
}


