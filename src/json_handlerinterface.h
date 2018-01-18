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
    void signHistoryReadWritePact(HistoryHandlerData *in_HistoryMegaObject);
    Q_INVOKABLE void runRecording();
protected:
    HistoryHandlerData *historySameData;
private:
    const std::string currentDateTime();
};

#endif // JSON_HANDLERINTERFACE_H
