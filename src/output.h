#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>


void outputResult(
const std::string &ip,
const std::string &result,
bool useJson,
bool useXml
);

void printXmlHeader();
void printXmlFooter();
void outputXmlToConsole(
const std::string &ip,
const std::string &result
);

#endif
