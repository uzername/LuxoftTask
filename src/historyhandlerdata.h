#ifndef HISTORYHANDLERDATA_H
#define HISTORYHANDLERDATA_H
#include "chesspiecemetadata.h"
#include "moredefs.h"
//===Structures for movement history
/**
  * An initial state of single chess piece. It's Enough data to create ChessPieceOnField
  */
typedef struct History_SingleInitialStateOfFigurine {
    ChessIntegerCoordType XcoordOnField;
    ChessIntegerCoordType YcoordOnField;
    ChessPieceBehaviorTypes BehaviorType;
    std::string PathToImage;
    ChessPieceSideTypes SideType;
    uint16_t uniqueIndexOfFigurine;
} History_SingleInitialStateOfFigurine;
/**
  * a single movement in history.
  * @see https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
  */
//unique index of figurine equals to the index of figurine in history_initialChessPiecesPositions vector.
//elements are not removed from it during gameplay
typedef struct History_SingleMovement {
    uint16_t uniqueIndexOfFigurine;
    ChessIntegerCoordType startX;
    ChessIntegerCoordType startY;
    ChessIntegerCoordType endX;
    ChessIntegerCoordType endY;
    uint8_t capturePerformed;
    uint16_t capturedUniqueIndex;
} History_SingleMovement;
/**
 * @brief The HistoryHandlerData class is used to store the current history of moves
 */
class HistoryHandlerData {
protected:
    std::vector<History_SingleInitialStateOfFigurine> history_initialChessPiecesPositions;
    std::vector<History_SingleMovement> history_setOfMovements;
public:
    HistoryHandlerData();
    std::vector<History_SingleInitialStateOfFigurine>::iterator getStatesVectorIteratorBegin();
    std::vector<History_SingleInitialStateOfFigurine>::iterator getStatesVectorIteratorEnd();
    void addInitialState(History_SingleInitialStateOfFigurine in_state);
    std::vector<History_SingleMovement>::iterator getMovementsVectorIteratorBegin();
    std::vector<History_SingleMovement>::iterator getMovementsVectorIteratorEnd();
    void addSingleMovement(History_SingleMovement in_SingleMovement);
    void quickCleanupAllHistory();
};

//extern HistoryHandlerData currentGlobalHistory;

#endif // HISTORYHANDLERDATA_H
