//THE WHOLE FILE IS QT-INDEPENDANT
#ifndef CHESSPIECEMETADATA_H
#define CHESSPIECEMETADATA_H
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
//use this type to represent coords
typedef uint8_t ChessIntegerCoordType;
typedef enum ChessPieceBehaviorTypes {
    KING_TYPE, QUEEN_TYPE, ROOK_TYPE, KNIGHT_TYPE, BISHOP_TYPE, BLACK_PAWN_TYPE, WHITE_PAWN_TYPE, PAWN_TYPE
} ChessPieceBehaviorTypes;
typedef enum ChessPiecePatternTypes {
    VECTOR_TYPE, POINT_TYPE
} ChessPiecePatternTypes;
typedef enum ChessPieceSideTypes { BLACK, WHITE } ChessPieceSideTypes;
/**
  * some predefined directions where the chess piece may move and attack.
  * TODO: declare Brasankham resolver for vectors. I guess this'll be kept for legacy
  */
typedef enum ChessPiecePatternVectorTypes {
    NORTH, SOUTH, EAST, WEST, NORTHEAST, SOUTHEAST, SOUTHWEST, NORTHWEST
} ChessPiecePatternVectorTypes;

/**
 * @brief The ChessPiecePattern class declares how the chess piece moves and how it may attack
 */
class ChessPiecePattern {
protected:
    //RTTI is simpler in C++ if you use this approach
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
    //direction is set in constructor
    ChessPiecePatternVectorTypes getTheDirectionType() const;
};
/**
 * @brief The ChessPiecePointPattern class represents that a figurine moves or attacks to a position defined by point, relative to current location of this figurine
 */
class ChessPiecePointPattern: public ChessPiecePattern {
protected:
    /*
    int XPoint;
    int YPoint;
    */
public:
    ChessPiecePointPattern(int in_XPoint, int in_YPoint);
    int XPoint;
    int YPoint;
    int getXPoint() const;
    int getYPoint() const;
};

/**
 * @brief The ChessPieceMetadataBehavior base class declares how a figurine moves and attacks
 */
class ChessPieceMetadataBehavior {
protected:
    //RTTI is simpler in C++ if you use this approach
    //indicates a chess figure for which the behavior is being declared
    ChessPieceBehaviorTypes currentBehaviorType;
    //may be unused
    ChessPieceSideTypes currentSideType;
    //polymorphism games are bad for memory
    /*
    std::vector<ChessPiecePattern> currentMovementPattern;
    std::vector<ChessPiecePattern> currentAttackPattern;
    */
    std::vector<ChessPiecePointPattern> currentPointMovementPattern;
    std::vector<ChessPieceVectorPattern> currentVectorMovementPattern;
    std::vector<ChessPiecePointPattern> currentPointAttackPattern;
    std::vector<ChessPieceVectorPattern> currentVectorAttackPattern;
public:
    ChessPieceMetadataBehavior();
    ChessPieceBehaviorTypes getCurrentBehaviorType() const;
    void setCurrentBehaviorType(const ChessPieceBehaviorTypes &value);
    ChessPieceSideTypes getCurrentSideType() const;
    void setCurrentSideType(const ChessPieceSideTypes &value);
    //declaring movement and attack patterns may be moved to constructor, and subclasses actually fill up these vectors in constructor
    //virtual void declareCurrentMovementPattern();
    std::vector<ChessPiecePointPattern>::iterator getMovementPatternArrayIterator();
    std::vector<ChessPiecePointPattern>::iterator getMovementPatternArrayIteratorEnd();
    std::vector<ChessPiecePointPattern>::iterator getAttackPatternArrayIterator();
    std::vector<ChessPiecePointPattern>::iterator getAttackPatternArrayIteratorEnd();
    /**
     * @brief performActionsAfterMovement - what actions (in code) should be performed after movement has been done.
     * it may be declared as a function ptr callback, but it's not worth it.
     */
    virtual void performActionsAfterMovement(void* ud);
    virtual void performActionsBeforeMovement(void* ud);
};


//seems like this is going to be unused
class ChessPieceMetadataDisplay {
protected:
    std::string pathToImage;
};

//a single place to define and keep behavior types of all possible chess pieces
//there are way too many exceptions from movement rules, so it may be better to declare each behavior type as separate class
class KingBehavior: public ChessPieceMetadataBehavior {
public:
    KingBehavior();
    void performActionsAfterMovement(void *ud) {    };
    void performActionsBeforeMovement(void *ud) {   };
};
class BlackPawnBehavior: public ChessPieceMetadataBehavior {
public:

    BlackPawnBehavior();
    void performActionsAfterMovement(void *ud);
    void performActionsBeforeMovement(void *ud);
};
//these instances are shared across all chess pieces!
extern KingBehavior* instKingBehavior;
extern BlackPawnBehavior* instBlackPawnBehavior;

void initBehaviors();
void deinitBehaviors();
#endif // CHESSPIECEMETADATA_H
