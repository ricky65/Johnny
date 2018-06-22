#ifndef RIFFPLAYER_H
#define RIFFPLAYER_H

#ifdef WIN32
#include <SDL_mixer.h>
#else
#include <SDL2/SDL_mixer.h>
#endif

#include <string>
#include <array>

#define MAX_AUDIO 24

namespace SCRANTIC {

class RIFFPlayer
{
protected:
	std::array<Mix_Chunk *, MAX_AUDIO> audioSamples = {};

public:
    explicit RIFFPlayer(const std::string &SCRExe);
    ~RIFFPlayer();
    void play(std::uint8_t num, bool stopAllOther = true);
};

}

#endif // RIFFPLAYER_H
