#include "historyhandlerdata.h"
HistoryHandlerData::HistoryHandlerData() {
    this->history_initialChessPiecesPositions = std::vector<History_SingleInitialStateOfFigurine>();
    this->history_setOfMovements = std::vector<History_SingleMovement>();
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
}

//HistoryHandlerData currentGlobalHistory;
