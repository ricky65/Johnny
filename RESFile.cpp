#include "RESFile.h"
#include "PALFile.h"
#include "BMPFile.h"
#include "SCRFile.h"
#include "TTMFile.h"
#include "ADSFile.h"

SCRANTIC::RESFile::RESFile(const std::string &name) : BaseFile(name)
{
    std::ifstream in;
    in.open(filename, std::ios::binary | std::ios::in);
    in.unsetf(std::ios::skipws);

    header.reserve(6);

    std::uint8_t byte;
    for (int i = 0; i < 6; ++i)
    {
        u_read_le(&in, byte);
        header.push_back(byte);
    }

    resFilename = read_string(&in, 12);

    if (resFilename.length() != 12)
        std::cerr << "RESFile: Resource filename corrupt? " << resFilename << std::endl;

    std::ifstream res;
    res.open(resFilename, std::ios::binary | std::ios::in);
    res.unsetf(std::ios::skipws);

    if (!res.is_open())
        std::cerr << "RESFile: Could not open resource file: " << resFilename << std::endl;

    u_read_le(&in, resCount);    

    for (int i = 0; i < resCount; ++i)
    {
		std::uint32_t blob, offset;    
		SCRANTIC::resource newRes;

        u_read_le(&in, blob);
        u_read_le(&in, offset);

        res.seekg(offset, std::ios::beg);

        newRes.num = i;
        newRes.filename = read_string(&res, 12);
        newRes.filetype = newRes.filename.substr(newRes.filename.rfind('.')+1);
        newRes.blob = blob;
        newRes.offset = offset;
        u_read_le(&res, newRes.size);

        for (std::uint32_t j = 0; j < newRes.size; ++j)
        {
            u_read_le(&res, byte);
            newRes.data.push_back(byte);
        }

        if (newRes.filetype == "PAL")
        {
			newRes.handle = std::make_shared<PALFile>(newRes.filename, newRes.data);
        }
        else if (newRes.filetype == "SCR")
        {
			newRes.handle = std::make_shared<SCRFile>(newRes.filename, newRes.data);
        }
        else if (newRes.filetype == "BMP")
        {
			newRes.handle = std::make_shared<BMPFile>(newRes.filename, newRes.data);
        }
        else if (newRes.filetype == "TTM")
        {
			newRes.handle = std::make_shared<TTMFile>(newRes.filename, newRes.data);
        }
        else if (newRes.filetype == "ADS")
        {
			newRes.handle = std::make_shared<ADSFile>(newRes.filename, newRes.data);
            ADSFiles.push_back(newRes.filename);
        }

        resourceMap.insert(std::pair<std::uint8_t, SCRANTIC::resource>(i, newRes));

        //saveFile(newRes.data, newRes.filename, "res/");
    }

    res.close();
    in.close();
}


