#include "json_handlerinterface.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include <cstdio>
#include <time.h>

using namespace rapidjson;
JSON_HandlerInterface::JSON_HandlerInterface(): QObject() {

}

void JSON_HandlerInterface::recordAllHistoryDataToFile(std::__cxx11::string in_pathToFile) {
//write to file: http://rapidjson.org/md_doc_stream.html
//write using sax parser: http://rapidjson.org/md_doc_sax.html

    FILE* fp = fopen(in_pathToFile.c_str(), "wb");
    char writeBuffer[65536];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    Writer<FileWriteStream> writer(os);

    writer.StartObject();
    writer.Key("Description");
    writer.String("ChessData");
    writer.Key("TimeStamp");
    writer.String(currentDateTime().c_str());
    if (this->historySameData == nullptr) {return;}
    writer.StartArray();
        for (it=this->historySameData->getStatesVectorIteratorBegin(); it!=this->historySameData->getStatesVectorIteratorEnd(); it++) {

        }
    writer.EndArray();
    writer.EndObject();

    fclose(fp);
}

void JSON_HandlerInterface::signHistoryReadWritePact(HistoryHandlerData *in_HistoryMegaObject) {
    this->historySameData = in_HistoryMegaObject;
}

void JSON_HandlerInterface::runRecording() {
    this->recordAllHistoryDataToFile("out.json");
}

// https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string JSON_HandlerInterface::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
