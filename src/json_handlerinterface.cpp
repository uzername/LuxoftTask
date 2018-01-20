#include "json_handlerinterface.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"
#include <cstdio>
#include <time.h>

using namespace rapidjson;
// http://rapidjson.org/md_doc_sax.html#Reader
struct MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
    bool Null() { return true; }
    bool Bool(bool b) { return true; }
    bool Int(int i) {  return true; }
    bool Uint(unsigned u) {  return true; }
    bool Int64(int64_t i) {  return true; }
    bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
    bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
    bool String(const char* str, SizeType length, bool copy) {
        cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool StartObject() { cout << "StartObject()" << endl; return true; }
    bool Key(const char* str, SizeType length, bool copy) {
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
    bool StartArray() { cout << "StartArray()" << endl; return true; }
    bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};


JSON_HandlerInterface::JSON_HandlerInterface(): QObject() {
    JSONReaderState = EXPECT_GENERALSTART;
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
    if (this->historySameData == nullptr) {writer.EndObject(); fclose(fp); return;}
    writer.Key("h_iCPP");
    writer.StartArray();
        for (std::vector<History_SingleInitialStateOfFigurine>::iterator it=this->historySameData->getStatesVectorIteratorBegin(); it!=this->historySameData->getStatesVectorIteratorEnd(); it++) {
            writer.StartObject();
            writer.Key("XCOF"); writer.Int(it->XcoordOnField);
            writer.Key("YCOF"); writer.Int(it->YcoordOnField);
            writer.Key("BT"); writer.Int(it->BehaviorType);
            writer.Key("PTI"); writer.String(it->PathToImage.c_str());
            writer.Key("ST"); writer.Int(it->SideType);
            writer.Key("uIOF1"); writer.Int(it->uniqueIndexOfFigurine);
            writer.EndObject();
        }
    writer.EndArray();
    writer.Key("h_sOM");
    writer.StartArray();
        for (std::vector<History_SingleMovement>::iterator it=this->historySameData->getMovementsVectorIteratorBegin(); it!=this->historySameData->getMovementsVectorIteratorEnd(); it++) {
            writer.StartObject();
            writer.Key("uIOF"); writer.Int(it->uniqueIndexOfFigurine);
            writer.Key("sX"); writer.Int(it->startX); writer.Key("eX");  writer.Int(it->endX);
            writer.Key("sY"); writer.Int(it->startY); writer.Key("eY");  writer.Int(it->endY);
            writer.Key("cP"); writer.Int(it->capturePerformed); writer.Key("cUI"); writer.Int(it->capturedUniqueIndex);
            writer.EndObject();
        }
    writer.EndArray();
    writer.EndObject();

    fclose(fp);
}

void JSON_HandlerInterface::obtainAllHistoryDataFromFile(std::__cxx11::string in_pathToFile) {

}

void JSON_HandlerInterface::signHistoryReadWritePact(HistoryHandlerData *in_HistoryMegaObject) {
    this->historySameData = in_HistoryMegaObject;
}

void JSON_HandlerInterface::runRecording() {
    this->recordAllHistoryDataToFile("out.json");
}

void JSON_HandlerInterface::runReading() {
    this->obtainAllHistoryDataFromFile("in.json");
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
