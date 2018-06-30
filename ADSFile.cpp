#include "ADSFile.h"

SCRANTIC::ADSFile::ADSFile(const std::string &name, std::vector<std::uint8_t> &data)
    : BaseFile(name)
{
    std::vector<std::uint8_t>::iterator it = data.begin();

    std::string tmp = read_const_string(it, 4);
    if (tmp != "VER:")
    {
        std::cerr << filename << ": \"VER:\" expected; got" << tmp << std::endl;
        return;
    }

    u_read_le(it, verSize);
    read_string(it, verSize-1);

    tmp = read_const_string(it, 4);
    if (tmp != "ADS:")
    {
        std::cerr << filename << ": \"ADS:\" expected; got" << tmp << std::endl;
        return;
    }

    u_read_le(it, resScrTagSize);

    tmp = read_const_string(it, 4);
    if (tmp != "RES:")
    {
        std::cerr << filename << ": \"RES:\" expected; got" << tmp << std::endl;
        return;
    }

    u_read_le(it, resSize);
    u_read_le(it, resCount);

    std::uint16_t id;
    std::string desc;

    for (std::uint16_t i = 0; i < resCount; ++i)
    {
        u_read_le(it, id);
        desc = read_string(it);
        resList.insert(std::pair<std::uint16_t, std::string>(id, desc));
    }


    tmp = read_const_string(it, 4);
    if (tmp != "SCR:")
    {
        std::cerr << filename << ": \"SCR:\" expected; got" << tmp << std::endl;
        return;
    }

    u_read_le(it, scrSize);
    scrSize -= 5; // substract compressionFlag and uncompressedSize
    u_read_le(it, compressionFlag);
    u_read_le(it, uncompressedSize);

    size_t i = std::distance(data.begin(), it);

    switch (compressionFlag)
    {
    case 0x00: rawScript = std::vector<std::uint8_t>(it, (it+scrSize)); break;
    case 0x01: rawScript = RLEDecompress(data, i, uncompressedSize); break;
    case 0x02: rawScript = LZWDecompress(data, i, uncompressedSize); break;
    case 0x03: rawScript = RLE2Decompress(data, i, uncompressedSize); break;
    default: std::cerr << filename << ": unhandled compression type: " << (int16_t)compressionFlag << std::endl;
    }

    if (uncompressedSize != (std::uint32_t)rawScript.size())
        std::cerr << filename << ": decompression error: expected size: " << (size_t)uncompressedSize  << " - got " << rawScript.size() << std::endl;

    std::advance(it, scrSize);

    /*if (rawScript.empty())
        return;*/

    tmp = read_const_string(it, 4);
    if (tmp != "TAG:")
    {
        std::cerr << filename << ": \"TAG:\" expected; got" << tmp << std::endl;
        return;
    }

    u_read_le(it, tagSize);
    u_read_le(it, tagCount);

    for (std::uint16_t i = 0; i < tagCount; ++i)
    {
        u_read_le(it, id);
        desc = read_string(it);
        tagList.insert(std::pair<std::uint16_t, std::string>(id, desc));
    }

    if (rawScript.empty())
        return;

    it = rawScript.begin();

    std::uint16_t word, word2, movie, leftover;   
    std::map<std::uint16_t, std::string>::iterator tagIt;
    std::map<std::pair<std::uint16_t, std::uint16_t>, size_t> currentLabels;

    movie = 0;
    leftover = 0;

    bool first = true;

    while (it != rawScript.end())
    {
        if (!leftover)
            u_read_le(it, word);
        else
        {
            word = leftover;
            leftover = 0;
        }

		Command command;
        command.opcode = word;

        switch (command.opcode)
        {
        case CMD_UNK_1070:
        case CMD_ADD_INIT_TTM:
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            break;
        case CMD_TTM_LABEL:
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word2);
            command.data.push_back(word2);
            u_read_le(it, leftover);
            currentLabels.insert(std::make_pair(std::make_pair(word, word2), script[movie].size()));
            while (leftover == CMD_OR)
            {
                u_read_le(it, leftover);
                if (leftover != CMD_TTM_LABEL)
                {
                    std::cerr << filename << ": Error processing SKIP command! Next word not skip: "<< leftover << std::endl;
                    break;
                }
                u_read_le(it, word);
                command.data.push_back(word);
                u_read_le(it, word2);
                command.data.push_back(word2);
                currentLabels.insert(std::make_pair(std::make_pair(word, word2), script[movie].size()));
                u_read_le(it, leftover);
            }
            break;
        case CMD_SKIP_IF_LAST:
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word2);
            command.data.push_back(word2);
            u_read_le(it, leftover);
            while (leftover == CMD_OR_SKIP)
            {
                u_read_le(it, leftover);
                if (leftover != CMD_SKIP_IF_LAST)
                {
                    std::cerr << filename << ": Error processing SKIP command! Next word not skip: "<< leftover << std::endl;
                    break;
                }
                u_read_le(it, word);
                command.data.push_back(word);
                u_read_le(it, word2);
                command.data.push_back(word2);
                u_read_le(it, leftover);
            }
            break;
        case CMD_UNK_1370:
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            break;
        case CMD_PLAY_MOVIE:
            break;
        case CMD_UNK_1520:
            break;
            /*u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            break;*/
        case CMD_ADD_TTM:
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            break;
        case CMD_KILL_TTM:
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            u_read_le(it, word);
            command.data.push_back(word);
            break;
        case CMD_RANDOM_START:
            break;
        case CMD_UNK_3020:
            u_read_le(it, word);
            command.data.push_back(word);
            break;
        case CMD_RANDOM_END:
        case CMD_UNK_4000:
        case CMD_UNK_F010:
            break;
        case CMD_PLAY_ADS_MOVIE:
            u_read_le(it, word);
            command.data.push_back(word);
            break;
        case CMD_UNK_FFFF:
            break;
        default:
            if (command.opcode >= 0x100)
                std::cerr << "Unkown ADS command " << command.opcode << std::endl;

            tagIt = tagList.find(command.opcode);
            if (tagIt != tagList.end())
                command.name = tagIt->second;

            if (first)
                first = false;
            else
            {
                labels.insert(std::make_pair(movie, currentLabels));
                currentLabels.clear();
            }

            movie = command.opcode;
            command.data.push_back(command.opcode);
            command.opcode = CMD_SET_SCENE;

            break;
        }

        script[movie].push_back(command);
    }

    labels.insert(std::make_pair(movie, currentLabels));

}

std::string SCRANTIC::ADSFile::getResource(std::uint16_t num)
{
    auto it = resList.find(num);
    if (it == resList.end())
        return "";
    else
        return it->second;
}

std::vector<SCRANTIC::Command> SCRANTIC::ADSFile::getFullMovie(std::uint16_t num)
{
    std::map<std::uint16_t, std::vector<Command> >::iterator it = script.find(num);
    if (it == script.end())
        return std::vector<Command>();
    else
        return it->second;
}

std::map<std::pair<std::uint16_t, std::uint16_t>, size_t> SCRANTIC::ADSFile::getMovieLabels(std::uint16_t num)
{
    std::map<std::uint16_t, std::map<std::pair<std::uint16_t, std::uint16_t>, size_t> >::iterator it = labels.find(num);
    if (it == labels.end())
        return std::map<std::pair<std::uint16_t, std::uint16_t>, size_t>();
    else
        return it->second;
}
