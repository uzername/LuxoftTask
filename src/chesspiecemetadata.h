#ifndef CHESSPIECEMETADATA_H
#define CHESSPIECEMETADATA_H
#include <stdint.h>

typedef enum ChessPieceBehaviorTypes {
    KING_TYPE, QUEEN_TYPE, ROOK_TYPE, KNIGHT_TYPE, BISHOP_TYPE, PAWN_TYPE
} ChessPieceBehaviorTypes;
typedef enum ChessPiecePatternTypes {
    VECTOR_TYPE, POINT_TYPE
} ChessPiecePatternTypes;
/**
  * some predefined directions where the chess piece may move and attack.
  * TODO: declare Brasankham resolver for vectors. I guess this'll be kept for legacy
  */
typedef enum ChessPiecePatternVectorTypes {
    NORTH, SOUTH, EAST, WEST, NORTHEAST, SOUTHEAST, SOUTHWEST, NORTHWEST
} ChessPiecePatternVectorTypes;
/**
 * @brief The ChessPieceMetadataBehavior base class declares how a figurine moves and attacks
 */
class ChessPieceMetadataBehavior {
protected:
    //no RTTI in C++, using this
    ChessPieceBehaviorTypes currentBehaviorType;
public:
    ChessPieceMetadataBehavior();
    ChessPieceBehaviorTypes getCurrentBehaviorType() const;
    void setCurrentBehaviorType(const ChessPieceBehaviorTypes &value);
};

/**
 * @brief The ChessPiecePattern class declares how the chess piece moves and how it may attack
 */
class ChessPiecePattern {
protected:
    //no RTTI in C++, using this
    ChessPiecePatternTypes instanceChessPiecePatternType;
public:

    ChessPiecePatternTypes getInstanceChessPiecePatternType() const;
    void setInstanceChessPiecePatternType(const ChessPiecePatternTypes &value);
};

class ChessPieceVectorPattern: public ChessPiecePattern {
protected:
    ChessPiecePatternVectorTypes theDirectionType;
public:
    ChessPieceVectorPattern(ChessPiecePatternVectorTypes in_DirectionType);

}
#endif // CHESSPIECEMETADATA_H
