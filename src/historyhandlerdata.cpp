#include "historyhandlerdata.h"
HistoryHandlerData::HistoryHandlerData() {
    this->history_initialChessPiecesPositions = std::vector<History_SingleInitialStateOfFigurine>();
    this->history_setOfMovements = std::vector<History_SingleMovement>();
}

std::vector::iterator HistoryHandlerData::getStatesVectorIteratorBegin() {
    return this->history_initialChessPiecesPositions.begin();
}

std::vector::iterator HistoryHandlerData::getStatesVectorIteratorEnd() {
    return this->history_initialChessPiecesPositions.end();
}

//HistoryHandlerData currentGlobalHistory;
