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

ChessIntegerCoordType ChessPiecePointPattern::getXPoint() const
{
    return XPoint;
}
