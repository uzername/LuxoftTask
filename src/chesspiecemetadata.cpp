#include "chesspiecemetadata.h"

ChessPieceBehaviorTypes ChessPieceMetadataBehavior::getCurrentBehaviorType() const
{
    return currentBehaviorType;
}

void ChessPieceMetadataBehavior::setCurrentBehaviorType(const ChessPieceBehaviorTypes &value)  {
    currentBehaviorType = value;
}

ChessPieceMetadataBehavior::ChessPieceMetadataBehavior() {

}

ChessPiecePatternTypes ChessPiecePattern::getInstanceChessPiecePatternType() const
{
    return instanceChessPiecePatternType;
}

void ChessPiecePattern::setInstanceChessPiecePatternType(const ChessPiecePatternTypes &value)
{
    instanceChessPiecePatternType = value;
}

ChessPieceVectorPattern::ChessPieceVectorPattern(ChessPiecePatternVectorTypes in_DirectionType)
{
    this->instanceChessPiecePatternType = VECTOR_TYPE;
    this->theDirectionType = in_DirectionType;
}
