#ifndef TTMFILE_H
#define TTMFILE_H

#include "BaseFile.h"

#include <map>

namespace SCRANTIC {

class TTMFile : public BaseFile
{
protected:
    std::uint32_t verSize;
    std::string version;
    std::uint32_t pagSize; // always 2 ?
    std::uint16_t pag;
    std::uint32_t ttmSize;
    std::uint8_t compressionFlag;
    std::uint32_t uncompressedSize;
    std::uint16_t fullTagSize;
    std::uint16_t magic; // 0x8000
    std::uint32_t tagSize;
    std::uint16_t tagCount;
    std::map<std::uint16_t, std::string> tagList;
    std::vector<std::uint8_t> rawScript;
    std::map<std::uint16_t, std::vector<Command> > script;

public:
    TTMFile(std::string name, std::vector<std::uint8_t> &data);
    std::vector<Command> getFullScene(std::uint16_t num);
    std::string getTag(std::uint16_t num);
    bool hasInit();
};

}

#endif // TTMFILE_H
