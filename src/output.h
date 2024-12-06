#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>

// Outputs results in different formats
void outputResult(const std::string &ip, const std::string &result, bool useJson, bool useXml);

// XML-specific helpers
void printXmlHeader();
void printXmlFooter();
void outputXmlToConsole(const std::string &ip, const std::string &result);

#endif // OUTPUT_H
