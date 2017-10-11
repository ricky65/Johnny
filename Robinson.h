#ifndef ROBINSON_H
#define ROBINSON_H

#include "RESFile.h"
#include "ADSFile.h"
#include "SCRFile.h"
#include "BMPFile.h"
#include "PALFile.h"
#include "TTMPlayer.h"
#include "RIFFPlayer.h"

#ifdef WIN32
#include <SDL_ttf.h>
#else
#include <SDL2/SDL_ttf.h>
#endif

namespace SCRANTIC {

class Robinson
{
protected:
    //menu stuff
    std::map<std::string, SDL_Texture *> menuScreen;
    std::map<std::string, std::map<std::uint16_t, SDL_Rect> > menuPos;
    std::string currentMenuScreen;
    size_t currentMenuPos;
    bool renderMenu;

    RESFile *res;
    ADSFile *ads;
    std::vector<Command> script;
    size_t scriptPos;
    std::map<std::pair<std::uint16_t, std::uint16_t>, size_t> labels;

    RIFFPlayer *audioPlayer;

    SDL_Renderer *renderer;
    SDL_Texture *rendererTarget;

    SDL_Color *palette;

    int8_t animationCycle;
    int8_t islandPos;
    SDL_Point islandTrunk;

    bool islandNight;
    bool islandLarge;
    bool movieRunning;

    // need to be freed
    SDL_Texture *bgTexture;
    SDL_Texture *fgTexture;
    SDL_Texture *saveTexture;

    //freed by SCRFile
    SDL_Texture *oceanTexture;
    SDL_Texture *oceanNightTexture;
    SDL_Texture *scrTexture;

    //freed by BMPFile
    BMPFile *backgroundBMP;
    BMPFile *holidayBMP;
    BMPFile *raftBMP;

    std::list<std::pair<std::uint16_t, std::uint16_t> > lastTTMs;

    // lots of rects...
    SDL_Rect fullRect;
    SDL_Rect oceanRect;
    SDL_Rect screenRect;

    //std::uint16_t currentMovie;
    std::string name;

    std::list<TTMPlayer *> ttmScenes;

    BMPFile *images[MAX_IMAGES];

    std::uint16_t delay;
    std::uint16_t currentMovie;
    std::uint16_t queuedMovie;
    size_t queuedPos;

    std::uint32_t delayTicks;

    void resetPlayer();

    void menuRenderer();
    void renderBackgroundAtPos(std::uint16_t num, int32_t x, int32_t y, bool raft = false, bool holiday = false);
    void animateBackground();
    void displaySplash();

    bool setPosToLabel(std::pair<std::uint16_t, std::uint16_t> lastPlayed);
    void addTTM(Command cmd);
    void runTTMs();

public:
    Robinson(std::string ResMap, std::string ScrExe);
    ~Robinson();

    bool navigateMenu(SDL_Keycode key);

    void initMenu(TTF_Font *font);
    void initRenderer(SDL_Renderer *rendererSDL);

    void advanceADSScript(std::pair<std::uint16_t, std::uint16_t> lastPlayed = std::make_pair((std::uint16_t)0,(std::uint16_t)0));
    void advanceScripts();
    void render();

    bool isMenuOpen() { return renderMenu; }
    bool isMovieRunning() { return movieRunning; }

    bool loadMovie(std::string adsName, std::uint16_t num);
    void startMovie();

    std::uint32_t getCurrentDelay() { return delay; }
    void displayMenu(bool show) { renderMenu = show; }
};

}

#endif // ROBINSON_H
