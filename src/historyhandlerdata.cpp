#include "historyhandlerdata.h"
HistoryHandlerData::HistoryHandlerData() {
    this->history_initialChessPiecesPositions = std::vector<History_SingleInitialStateOfFigurine>();
    this->history_setOfMovements = std::vector<History_SingleMovement>();
}
