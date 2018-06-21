#ifndef PALFILE_H
#define PALFILE_H

#include "BaseFile.h"
#include <tuple>

namespace SCRANTIC {

class PALFile : public BaseFile
{
protected:
    std::uint16_t vgaSize;
    std::uint16_t magic;  // 0x8000
    std::uint32_t palCount;
    //std::vector< std::tuple< std::uint8_t, std::uint8_t, std::uint8_t > > palette;
    std::vector<SDL_Color> palette;
public:
    PALFile(const std::string &name, std::vector<std::uint8_t> &data);
    //std::vector< std::tuple< std::uint8_t, std::uint8_t, std::uint8_t > > getPalette() { return palette; }
    SDL_Color *getPalette() { return palette.data(); }
};

}

#endif // PALFILE_H
