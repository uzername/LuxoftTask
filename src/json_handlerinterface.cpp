#include "json_handlerinterface.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include <cstdio>

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
    writer.Key("hello");
    writer.String("world");
    writer.EndObject();

    fclose(fp);
}

void JSON_HandlerInterface::runRecording() {
    this->recordAllHistoryDataToFile("out.json");
}
