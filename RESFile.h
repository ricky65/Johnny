#ifndef RESFILE_H
#define RESFILE_H

#include "BaseFile.h"
#include <map>

namespace SCRANTIC {

struct resource
{
    std::uint16_t num;
    std::string filename;
    std::uint32_t blob;
    std::uint32_t offset;
    std::uint32_t size;
    std::vector<std::uint8_t> data;
    std::string filetype;
    BaseFile *handle;
};

class RESFile : public BaseFile
{
protected:
    std::vector<std::uint8_t> header;
    std::uint16_t resCount;

public:
    RESFile(std::string name);
    ~RESFile();
    std::map<std::uint8_t, SCRANTIC::resource> resourceMap;
    std::string resFilename;
    std::vector<std::string> ADSFiles;
    BaseFile *getResource(std::string name);
};

}

#endif // RESFILE_H
