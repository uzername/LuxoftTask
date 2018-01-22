#include "historyhandlerdata.h"
HistoryHandlerData::HistoryHandlerData() {
    this->history_initialChessPiecesPositions = std::vector<History_SingleInitialStateOfFigurine>();
    this->history_setOfMovements = std::vector<History_SingleMovement>();
    this->currentDisplayedMove = 0;
}

std::vector<History_SingleInitialStateOfFigurine>::iterator HistoryHandlerData::getStatesVectorIteratorBegin() {
    return this->history_initialChessPiecesPositions.begin();
}

std::vector<History_SingleInitialStateOfFigurine>::iterator HistoryHandlerData::getStatesVectorIteratorEnd() {
    return this->history_initialChessPiecesPositions.end();
}

void HistoryHandlerData::addInitialState(History_SingleInitialStateOfFigurine in_state) {
    this->history_initialChessPiecesPositions.push_back(in_state);
}

uint16_t HistoryHandlerData::getSizeInitialStates() {
    return this->history_initialChessPiecesPositions.size();
}

std::vector<History_SingleMovement>::iterator HistoryHandlerData::getMovementsVectorIteratorBegin() {
    return this->history_setOfMovements.begin();
}

std::vector<History_SingleMovement>::iterator HistoryHandlerData::getMovementsVectorIteratorEnd() {
    return this->history_setOfMovements.end();
}

void HistoryHandlerData::addSingleMovement(History_SingleMovement in_SingleMovement) {
    this->history_setOfMovements.push_back(in_SingleMovement);
}

void HistoryHandlerData::quickCleanupAllHistory() {
    this->history_initialChessPiecesPositions.clear();
    this->history_setOfMovements.clear();
    this->currentDisplayedMove = 0;
}

uint16_t HistoryHandlerData::prevMoveStep() {
    if ((this->currentDisplayedMove > 0)) {
        this->currentDisplayedMove--;
    }
    return this->currentDisplayedMove;
}

uint16_t HistoryHandlerData::nextMoveStep() {
    if (this->history_setOfMovements.size()==0) {
        return 0;
    }
    if (this->history_setOfMovements.size() > this->currentDisplayedMove) {
        this->currentDisplayedMove++;
    }
    return this->currentDisplayedMove;
}

History_SingleMovement *HistoryHandlerData::getcurrentMovementPtrByCurrentIndex() {
    if (this->currentDisplayedMove == 0) { return nullptr; }
    History_SingleMovement* movementToReturn = new History_SingleMovement;
    *movementToReturn = history_setOfMovements.at(this->currentDisplayedMove-1);
    return movementToReturn;
}

//HistoryHandlerData currentGlobalHistory;
