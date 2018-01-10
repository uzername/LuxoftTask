#include "chesspiecemetadata.h"

ChessPieceBehaviorTypes ChessPieceMetadataBehavior::getCurrentBehaviorType() const
{
    return currentBehaviorType;
}

void ChessPieceMetadataBehavior::setCurrentBehaviorType(const ChessPieceBehaviorTypes &value)  {
    currentBehaviorType = value;
}

ChessPieceSideTypes ChessPieceMetadataBehavior::getCurrentSideType() const
{
    return currentSideType;
}

void ChessPieceMetadataBehavior::setCurrentSideType(const ChessPieceSideTypes &value)
{
    currentSideType = value;
}

ChessPieceMetadataBehavior::ChessPieceMetadataBehavior() {
    this->currentMovementPattern = std::vector<ChessPiecePattern>();
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

ChessIntegerCoordType ChessPiecePointPattern::getYPoint() const
{
    return YPoint;
}

ChessPiecePointPattern::ChessPiecePointPattern(ChessIntegerCoordType in_XPoint, ChessIntegerCoordType in_YPoint)
{
    this->instanceChessPiecePatternType = POINT_TYPE;
    this->XPoint = in_XPoint;
    this->YPoint = in_YPoint;
}

ChessIntegerCoordType ChessPiecePointPattern::getXPoint() const {
    return XPoint;
}

KingBehavior::KingBehavior()
    :ChessPieceMetadataBehavior() {
    this->currentBehaviorType = KING_TYPE;
}

//========
std::shared_ptr<KingBehavior> instKingBehavior;
void initBehaviors() {
    KingBehavior KingBehaviorInstance = KingBehavior();
     instKingBehavior = std::shared_ptr<KingBehavior>(&KingBehaviorInstance);
}
