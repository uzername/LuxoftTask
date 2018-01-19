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

std::vector<History_SingleMovement>::iterator HistoryHandlerData::getMovementsVectorIteratorBegin() {
    return this->history_setOfMovements.begin();
}

std::vector<History_SingleMovement>::iterator HistoryHandlerData::getMovementsVectorIteratorEnd() {
    return this->history_setOfMovements.end();
}

//HistoryHandlerData currentGlobalHistory;
