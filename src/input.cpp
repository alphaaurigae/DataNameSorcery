#include "input.h"
#include <stdexcept>
#include <filesystem>
#include <algorithm>
#include <Poco/Exception.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/Net/SocketAddress.h>

namespace fs = std::filesystem;

FileReader::FileReader(const std::string &filename)
{
    if (!fs::exists(filename))
    {
        throw std::runtime_error("Error: File " + filename + " does not exist");
    }

    if (!fs::is_regular_file(filename))
    {
        throw std::runtime_error("Error: " + filename + " is not a regular file");
    }

    file = std::make_unique<std::ifstream>(filename, std::ios::binary);
    if (!file->is_open())
    {
        throw std::runtime_error("Error: Unable to open " + filename);
    }
}

std::string FileReader::readFileLines() const
{
    std::string lines;
    std::string line;

    while (std::getline(*file, line))
    {
        processLine(line, lines);
    }

    return lines;
}

void FileReader::processLine(std::string line, std::string &lines) const
{
    auto pos = line.find("//");
    if (pos != std::string::npos)
    {
        line.erase(pos);
    }

    line.erase(line.begin(), std::find_if_not(line.begin(), line.end(), ::isspace));
    line.erase(std::find_if_not(line.rbegin(), line.rend(), ::isspace).base(), line.end());

    if (!line.empty())
    {
        try
        {
            Poco::Net::IPAddress ip(line);
            if (ip.family() == Poco::Net::IPAddress::IPv4 || ip.family() == Poco::Net::IPAddress::IPv6)
            {
                lines += line + '\n';
            }
            else
            {
                Poco::Net::SocketAddress address(line, 80);
                lines += line + '\n';
            }
        }
        catch (const Poco::Exception &)
        {
            lines += "Invalid input: " + line + '\n';
        }
    }
}