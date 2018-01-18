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
} History_SingleInitialStateOfFigurine;
/**
  * a single movement in history.
  * @see https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
  */
typedef struct History_SingleMovement {
    uint16_t uniqueIndexOfFigurine;
    ChessIntegerCoordType startX;
    ChessIntegerCoordType startY;
    ChessIntegerCoordType endX;
    ChessIntegerCoordType endY;
    uint8_t capturePerformed;
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
};

//extern HistoryHandlerData currentGlobalHistory;

#endif // HISTORYHANDLERDATA_H
