#ifndef JSON_HANDLERINTERFACE_H
#define JSON_HANDLERINTERFACE_H

#include "moredefs.h"
#include "historyhandlerdata.h"
#include <QtCore>
class JSON_HandlerInterface: public QObject
{
    Q_OBJECT
public:
    JSON_HandlerInterface();
    void recordAllHistoryDataToFile(std::string in_pathToFile);
    Q_INVOKABLE void runRecording();
};

#endif // JSON_HANDLERINTERFACE_H
