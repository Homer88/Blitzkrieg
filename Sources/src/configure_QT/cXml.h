#ifndef CXML_H
#define CXML_H
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

class cXml{
private:
    QFile fileconfigure;
public:
    cXml();
    void openConfig();
    void closeConfig();
};



#endif // CXML_H
