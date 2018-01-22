#include "json_handlerinterface.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <cstdio>
#include <time.h>

using namespace rapidjson;
// http://rapidjson.org/md_doc_sax.html#Reader
#ifdef USE_SAXJSON_PARSER_SOMENAME
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
    bool StartObject() {  return true; }
    bool Key(const char* str, SizeType length, bool copy) {
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
    bool StartArray() { cout << "StartArray()" << endl; return true; }
    bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};
#endif

JSON_HandlerInterface::JSON_HandlerInterface(): QObject() {
    currentTimeStampFromFile = "";
    #ifdef USE_SAXJSON_PARSER_SOMENAME
    JSONReaderState = EXPECT_GENERALSTART;
    #endif
}

void JSON_HandlerInterface::recordAllHistoryDataToFile(std::__cxx11::string in_pathToFile) {
//write to file: http://rapidjson.org/md_doc_stream.html
//write using sax parser: http://rapidjson.org/md_doc_sax.html
//skip unnecessary characters
    FILE* fp = fopen(in_pathToFile.c_str()+8, "wb");
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
//http://rapidjson.org/md_doc_tutorial.html makes DOM. Not using streamed parser
void JSON_HandlerInterface::obtainAllHistoryDataFromFile(std::__cxx11::string in_pathToFile) {
    //this routine is called after clearGamefield, called in qml file. But still it would not hurt to remove all items from history.
    //we'll be recording history from file
        this->historySameData->quickCleanupAllHistory();
   // printf("STARTED OBTAINING\n");
      //QT adds unnecessary symbols to beginning, skip that
   // const char* tmpchr = in_pathToFile.c_str()+8;
    FILE* fp = fopen(in_pathToFile.c_str()+8, "rb"); // non-Windows use "r"
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    d.ParseStream(is);
    assert(d.HasMember("Description"));
    assert(d.HasMember("TimeStamp"));
    char readTimeStampFromFile[255]; sprintf(readTimeStampFromFile, "%s", d.GetObject()["TimeStamp"].GetString());
    this->currentTimeStampFromFile = QString(readTimeStampFromFile);
    assert(d.HasMember("h_iCPP"));
    const Value& h_iCPP = d["h_iCPP"];
    assert(h_iCPP.IsArray());
    for (Value::ConstValueIterator itr = h_iCPP.Begin(); itr != h_iCPP.End(); ++itr) {
       //GenericObject::Object historyPiecePosition = itr->GetObject();
        /*
       for (Value::ConstMemberIterator itr2 = itr->GetObject().MemberBegin(); itr2 != itr->GetObject().MemberEnd(); ++itr2) {
           printf("member %s \n", itr2->name.GetString());
       }
        */
        History_SingleInitialStateOfFigurine stateToBeAdded;
        stateToBeAdded.XcoordOnField = itr->GetObject()["XCOF"].GetInt();
        stateToBeAdded.YcoordOnField = itr->GetObject()["YCOF"].GetInt();
        stateToBeAdded.BehaviorType = ChessPieceBehaviorTypes( itr->GetObject()["BT"].GetInt() );
        char readPathToImage[255]; sprintf(readPathToImage, "%s", itr->GetObject()["PTI"].GetString());
        stateToBeAdded.PathToImage = std::string(readPathToImage); //itr->GetObject()["PTI"].GetString();
        stateToBeAdded.SideType = ChessPieceSideTypes( itr->GetObject()["ST"].GetInt() );
        stateToBeAdded.uniqueIndexOfFigurine = itr->GetObject()["uIOF1"].GetInt();
        this->historySameData->addInitialState(stateToBeAdded);
    }
    assert(d.HasMember("h_sOM"));
    const Value& h_sOM = d["h_sOM"];
    assert(h_sOM.IsArray());
    for (Value::ConstValueIterator itr = h_sOM.Begin(); itr!=h_sOM.End(); ++itr) {
        History_SingleMovement movementToBeAdded;
        movementToBeAdded.uniqueIndexOfFigurine = itr->GetObject()["uIOF"].GetInt();
        movementToBeAdded.startX = itr->GetObject()["sX"].GetInt();
        movementToBeAdded.endX = itr->GetObject()["eX"].GetInt();
        movementToBeAdded.startY = itr->GetObject()["sY"].GetInt();
        movementToBeAdded.endY = itr->GetObject()["eY"].GetInt();
        movementToBeAdded.capturePerformed = itr->GetObject()["cP"].GetInt();
        movementToBeAdded.capturedUniqueIndex = itr->GetObject()["cUI"].GetInt();
        this->historySameData->addSingleMovement(movementToBeAdded);
    }
    fclose(fp);
}

void JSON_HandlerInterface::signHistoryReadWritePact(HistoryHandlerData *in_HistoryMegaObject) {
    this->historySameData = in_HistoryMegaObject;
}

void JSON_HandlerInterface::runRecording(QString in_filepath) {
    this->recordAllHistoryDataToFile(in_filepath.toStdString());
}

void JSON_HandlerInterface::runReading(QString in_filepath) {
    this->obtainAllHistoryDataFromFile(in_filepath.toStdString());
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

QString JSON_HandlerInterface::getCurrentTimeStampFromFile() const
{
    return currentTimeStampFromFile;
}
