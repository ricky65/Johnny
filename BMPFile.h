#ifndef BMPFILE_H
#define BMPFILE_H

#include "BaseFile.h"

namespace SCRANTIC {

class BMPFile : public BaseFile
{
protected:
    std::uint16_t infBinSize;
    std::uint16_t magic;  //0x8000
    std::uint32_t infSize;
    std::uint16_t imageCount;
    std::vector<std::uint16_t> widthList;
    std::vector<std::uint16_t> heightList;
    std::uint32_t binSize;
    std::uint8_t compressionFlag;
    std::uint32_t uncompressedSize;
    std::vector<std::uint8_t> uncompressedData;
    std::vector<SDL_Surface *> imageList;
    std::vector<SDL_Rect> imageRect;
    SDL_Surface *overview;
    SDL_Texture *ovTexture;
    void createOverview();

public:
    BMPFile(std::string name, std::vector<std::uint8_t> &data);
    ~BMPFile();
    SDL_Texture *getImage(SDL_Renderer *renderer, std::uint16_t num, SDL_Rect &rect);
    size_t getImageCount() { return imageList.size(); }
    SDL_Texture *getOverviewImage(SDL_Renderer *renderer, SDL_Rect &rect);
    void setPalette(SDL_Color color[], std::uint16_t count);
    SDL_Rect getRect(std::uint16_t num);

};

}

#endif // BMPFILE_H

