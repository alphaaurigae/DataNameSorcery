#ifndef INPUT_H
#define INPUT_H

#include <fstream>
#include <memory>
#include <string>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

class FileReader {
public:
    explicit FileReader(const std::string& filename);
    std::string readFileLines() const;
    
private:
    void processLine(std::string line, std::string &lines) const;

    mutable std::unique_ptr<std::ifstream> file;
};

#endif