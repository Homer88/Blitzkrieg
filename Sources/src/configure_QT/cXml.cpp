#include "cXml.h"

cXml::cXml(){

}


cXml XmlFile;
void cXml::openConfig(){
    fileconfigure.setFileName("config.cfg");
    if (!fileconfigure.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"Не открылось.";
        return;
    }

}
void closeConfig(){

}
