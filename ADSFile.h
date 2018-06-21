#ifndef ADSFILE_H
#define ADSFILE_H

#include "BaseFile.h"
#include <map>
#include <tuple>

namespace SCRANTIC {

class ADSFile : public BaseFile
{
protected:
    std::uint32_t verSize;
    std::string version;
    std::uint32_t resScrTagSize;
    std::uint32_t resSize;
    std::uint16_t resCount;
    std::map<std::uint16_t, std::string> resList;
    std::uint32_t scrSize;
    std::uint8_t compressionFlag;
    std::uint32_t uncompressedSize;
    std::vector<std::uint8_t> rawScript;
    std::uint32_t tagSize;
    std::uint16_t tagCount;
    std::map<std::uint16_t, std::map<std::pair<std::uint16_t, std::uint16_t>, size_t> > labels;
    std::map<std::uint16_t, std::vector<Command> > script;

#ifdef DUMP_ADS
    friend class Robinson;
#endif

public:
    std::map<std::uint16_t, std::string> tagList;

    ADSFile(const std::string &name, std::vector<std::uint8_t> &data);
    std::string getResource(std::uint16_t num);
    std::vector<Command> getFullMovie(std::uint16_t num);
    std::map<std::pair<std::uint16_t, std::uint16_t>, size_t> getMovieLabels(std::uint16_t num);
};

}

#endif // ADSFILE_H
