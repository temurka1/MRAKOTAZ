#include "pch.h"
#include "BitmapReader.h"
#include "BitmapFrame.h"

#include "ResultOr.hpp"

namespace core = MRAKOTAZ::core;
namespace bitmap = MRAKOTAZ::bitmap;

using BitmapFrame = bitmap::BitmapFrame;

namespace
{
    /// <summary>
    /// bitmap file header
    /// </summary>
    struct bfh_t
    {
        unsigned short type;
        unsigned int   size;
        unsigned short reserved1;
        unsigned short reserved2;
        unsigned int   off_bits;

        unsigned int struct_size() const
        {
            return sizeof(type) + sizeof(size) + sizeof(reserved1) + sizeof(reserved2) + sizeof(off_bits);
        }
    };

    /// <summary>
    /// bitmap information header
    /// </summary>
    struct bih_t
    {
        unsigned int size;
        unsigned int width;
        unsigned int height;
        unsigned short planes;
        unsigned short bit_count;
        unsigned int compression;
        unsigned int size_image;
        unsigned int x_pels_per_meter;
        unsigned int y_pels_per_meter;
        unsigned int clr_used;
        unsigned int clr_important;

        unsigned int struct_size() const
        {
            return sizeof(size) + sizeof(width) + sizeof(height) + sizeof(planes) + sizeof(bit_count) + sizeof(compression) + 
                sizeof(size_image) + sizeof(x_pels_per_meter) + sizeof(y_pels_per_meter) + sizeof(clr_used) + sizeof(clr_important);
        }
    };

    /// <summary>
    /// Reads data from stream into T
    /// </summary>
    template <typename T> inline void read_from_stream(std::ifstream& stream, T& t)
    {
        stream.read(reinterpret_cast<char*>(&t), sizeof(T));
    }

    /// <summary>
    /// read bitmap file header
    /// </summary>
    inline void read_bfh(std::ifstream& stream, bfh_t& bfh)
    {
        read_from_stream(stream, bfh.type);
        read_from_stream(stream, bfh.size);
        read_from_stream(stream, bfh.reserved1);
        read_from_stream(stream, bfh.reserved2);
        read_from_stream(stream, bfh.off_bits);
    }

    /// <summary>
    /// read bitmap information header
    /// </summary>
    inline void read_bih(std::ifstream& stream, bih_t& bih)
    {
        read_from_stream(stream, bih.size);
        read_from_stream(stream, bih.width);
        read_from_stream(stream, bih.height);
        read_from_stream(stream, bih.planes);
        read_from_stream(stream, bih.bit_count);
        read_from_stream(stream, bih.compression);
        read_from_stream(stream, bih.size_image);
        read_from_stream(stream, bih.x_pels_per_meter);
        read_from_stream(stream, bih.y_pels_per_meter);
        read_from_stream(stream, bih.clr_used);
        read_from_stream(stream, bih.clr_important);
    }

    /// <summary>
    /// returns file size for opened file stream
    /// </summary>
    inline size_t file_size(std::ifstream& stream)
    {
        if (!stream)
        {
            return 0;
        }

        stream.seekg(0, std::ios::end);
        size_t size = static_cast<std::size_t>(stream.tellg());
        stream.seekg(0, std::ios::beg);

        return size;
    }
}

core::ResultOr<BitmapFrame> bitmap::read_bitmap_frame(const std::string& filename)
{
    std::ifstream stream(filename.c_str(), std::ios::binary);

    if (!stream)
    {
        return core::ResultOr<BitmapFrame>(core::Result(core::Result::ERROR));
    }

    std::size_t bitmapFileSize = file_size(stream);

    bfh_t bfh;
    bih_t bih;

    std::memset(&bfh, 0x00, sizeof(bfh_t));
    std::memset(&bih, 0x00, sizeof(bfh_t));

    read_bfh(stream, bfh);
    read_bih(stream, bih);

    if (bfh.type != 19778)
    {
        stream.close();

        // not really bitmap
        return core::ResultOr<BitmapFrame>(core::Result(core::Result::ERROR));
    }

    if (bih.bit_count != 24)
    {
        stream.close();

        // invalid bit depth - should be 24
        return core::ResultOr<BitmapFrame>(core::Result(core::Result::ERROR));
    }

    if (bih.size != bih.struct_size())
    {
        stream.close();

        // invalid BIH size
        return core::ResultOr<BitmapFrame>(core::Result(core::Result::ERROR));
    }

    uint32_t width = bih.width;
    uint32_t height = bih.height;

    uint8_t bpp = bih.bit_count >> 3;

    unsigned int padding = (4 - ((3 * width) % 4)) % 4;
    char paddingData[4] = { 0x00, 0x00, 0x00, 0x00 };

    std::size_t bitmapLogicalSize = (height * width * bpp) + (height * padding) + bih.struct_size() + bfh.struct_size();

    if (bitmapFileSize != bitmapLogicalSize)
    {
        stream.close();

        // logical and physical sizes mismatch
        return core::ResultOr<BitmapFrame>(core::Result(core::Result::ERROR));
    }

    std::vector<uint8_t> data;

    uint32_t rowIncrement = width * bpp;
    data.resize(height * rowIncrement);

    for (unsigned int i = 0; i < height; ++i)
    {
        uint8_t* dataPtr = const_cast<unsigned char*>(&data[((height - i - 1) * rowIncrement)]);

        stream.read(reinterpret_cast<char*>(dataPtr), sizeof(char) * bpp * width);
        stream.read(paddingData, padding);
    }

    stream.close();

    return core::ResultOr<BitmapFrame>(BitmapFrame(data, width, height, bpp));
}
