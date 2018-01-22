#ifndef JSON_HANDLERINTERFACE_H
#define JSON_HANDLERINTERFACE_H

#include "moredefs.h"
#include "historyhandlerdata.h"
#include <QtCore>
#ifdef USE_SAXJSON_PARSER_SOMENAME
typedef enum WhatToExpectWhenReadingJson {
    EXPECT_GENERALSTART, EXPECT_GENERALFINISH,
    EXPECT_DESCRIPTION,
    EXPECT_START_INITIALPOSITIONS_ARRAY,  EXPECT_END_INITIALPOSITIONS_ARRAY,
    EXPECT_START_INITIALPOSITION_OBJECT, EXPECT_END_INITIALPOSITION_OBJECT,
    EXPECT_XCOF, EXPECT_YCOF, EXPECT_BT, EXPECT_PTI, EXPECT_ST, EXPECT_UIOF1,

    EXPECT_START_ARRAY_MOVEMENTS, EXPECT_END_ARRAY_MOVEMENTS,
    EXPECT_SX, EXPECT_SY, EXPECT_EX, EXPECT_EY, EXPECT_UIOF, EXPECT_CP
} WhatToExpectWhenReadingJson;
#endif

class JSON_HandlerInterface: public QObject
{
    Q_OBJECT
public:
    JSON_HandlerInterface();
    void recordAllHistoryDataToFile(std::string in_pathToFile);
    void obtainAllHistoryDataFromFile(std::string in_pathToFile);
    void signHistoryReadWritePact(HistoryHandlerData *in_HistoryMegaObject);
    /**
      * Run recording to file
      */
    Q_INVOKABLE void runRecording(QString in_filepath);
    /**
      * Run reading from file
      */
    Q_INVOKABLE void runReading(QString in_filepath);
    Q_INVOKABLE QString getCurrentTimeStampFromFile() const;

protected:
    HistoryHandlerData *historySameData;
private:
    const std::string currentDateTime();
    #ifdef USE_SAXJSON_PARSER_SOMENAME
    WhatToExpectWhenReadingJson JSONReaderState;
    #endif
    QString currentTimeStampFromFile;
};

#endif // JSON_HANDLERINTERFACE_H
