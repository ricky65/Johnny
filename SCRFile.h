#ifndef SCRFILE_H
#define SCRFILE_H

#include "BaseFile.h"

namespace SCRANTIC {

class SCRFile : public BaseFile
{
protected:
    std::uint16_t dimBinSize; //
    std::uint16_t magic;  //0x8000
    std::uint32_t dimSize;
    std::uint16_t imageCount;
    std::uint16_t width;
    std::uint16_t height;
    std::uint32_t binSize;
    std::uint8_t compressionFlag;
    std::uint32_t uncompressedSize;
    std::vector<std::uint8_t> uncompressedData;
    SDL_Surface *image;
    SDL_Texture *texture;

public:
    SCRFile(std::string name, std::vector<std::uint8_t> &data);
    ~SCRFile();
    SDL_Texture *getImage(SDL_Renderer *renderer, SDL_Rect &rect);
    void setPalette(const SDL_Color *color, std::uint16_t count);
};

}

#endif // SCRFILE_H

