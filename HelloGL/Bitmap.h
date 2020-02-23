// PROJECT:										Bitmap Class - Fundamentals of Game and Graphical System Development (COSE40630)
// FILENAME:									Bitmap.h
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE CREATED:								09/02/2020
// DATE MODIFIED:
// LANGUAGE:									C++
// COMPILATION:									MSVC++ / MinGW C++ (S500 VS2019, Testing Qt Creator)
// INPUT:                                       Class can take a bitmap file as a parameter
// OUTPUT:                                      Capable of writing the file back to disk, writing back just the colour data or various error codes within the class
// PURPOSE:										Bitmap loader, for use with the OpenGL tutorials. Conversion of .bmp to .raw
// NOTES:
//                                              Please see the associated article regarding the format of a .bmp file.
//												http://en.wikipedia.org/wiki/BMP_file_format for an explanation of the bitmap file format
//
//												This program handles colour 24 - bit.bmp files with a BITMAPINFOHEADER type header
//												The image must be square - to be compatible with OpenGL the width and height must be
//												a power of 2 (16, 32, 64, 128, 256, 512, 1024)
//
//                                              Header definitions match those of Windows BITMAPFILEHEADER and BITMAPINFOHEADER but are provided to mitigate reliance on windows.h
//
//                                              The class only leverages the compulsory components of the Bitmap definition.
//
//                                              May improve the class with a static public interface
//
//                                              If target system cstdint does not typedef the required widths, specific types will have to be declared (likely signed/unsigned shorts and longs)
//
//                                              If file is unsupported, pixel array will not be processed however the headers will be available to the class public interface


// Change Log:


// Header Guard
#ifndef BITMAP_H
#define BITMAP_H


// Included Header Files (Alphabetical STL then followed by custom)
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>


// NAMESPACE:									FOGGS
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:										Use of namespace to avoid conflicts with other Bitmap named classes
// KEY VARIABLES:
// KEY FUNCTIONS AND LIBRARIES:
// MODIFICATIONS AND NOTES:
namespace FOGGS
{

    // CLASS:       								Bitmap
    // COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
    // VERSION:										1.0
    // DATE:                                        09/02/2020
    // PURPOSE:										Class providing functionality for a bitmap, including data structure defining its components
    // KEY VARIABLES:                               Expected File Type (BM) declared as const value 0x4D42 rather than definition in the global namespace
    // KEY FUNCTIONS AND LIBRARIES:
    // MODIFICATIONS AND NOTES:
    class Bitmap
    {
    public:
        // Enumeration Definition for returned error conditions (requires use of cast)
        // CONSTANT			VALUE			DESCRIPTION
        // --------------------------------------------------------------------------------
        // NOERR			0				No error encountered, standard return value
        // NOINPUTFILE		1				Error reading the input file, cannot be opened
        // NOSQUARE			2				Bitmap provided is not square
        // NOBMP			3				File is not a bitmap
        // READFILEHEAD     4               Error reading the file header (different to 3)
        // READINFOHEAD     5               Error reading the BITMAPINFOHEADER
        // READRAWDATA      6               Error reading the image data
        // NOOUTPUTFILE     7               Error opening file for output
        // WRITERAWDATA     8               Error writing the rgb data out
        // UNSUPPORTED      9               Unsupported bit depth, must be 24
        // NOHEADERVERS     10              Unsupported header version. BITMAPINFOHEADER
        // NOLOADED         11              No Bitmap loaded, export cannot be conducted
        enum class EnmBitmapErrors
        {
            NOERR,
            NOINPUTFILE,
            NOSQUARE,
            NOBMP,
            READFILEHEAD,
            READINFOHEAD,
            READRAWDATA,
            NOOUTPUTFILE,
            WRITERAWDATA,
            UNSUPPORTED,
            NOHEADERVERS,
            NOLOADED
        };


        // Enumeration Definition for compression methods (requires use of cast)
        // CONSTANT			VALUE			DESCRIPTION
        // --------------------------------------------------------------------------------
        // BI_RGB			0				No compression
        // BI_RLE8          1				RLE 8 bit
        // BI_RLE4			2				RLE 4 bit
        // BI_BITFIELDS		3				Huffman 1D
        // BI_JPEG          4               RLE 24 bit
        // BI_PNG           5
        // BI_ALPHABITFI... 6               RGBA Bit Field Masks
        // BI_CMYK          11              No Compression
        // BI_CMYKRLE8      12              RLE 8 bit
        // BI_CMYKRLE4      13              RLE 4 bit
        enum class EnmCompressionMethods
        {
            BI_RGB,
            BI_RLE8,
            BI_RLE4,
            BI_BITFIELDS,
            BI_JPEG,
            BI_PNG,
            BI_ALPHABITFIELDS,
            BI_CMYK = 11,
            BI_CMYKRLE8,
            BI_CMYKRLE4
        };


        // Preprocessor Statement START (condense structures)
        #pragma pack(push,1)


        // Structure Definition: buffer for red/green/blue values (RGB)
        // SIZE             PURPOSE
        // --------------------------------------------------------------------------------
        // 8                Red channel value
        // 8                Green channel value
        // 8                Blue channel value
        typedef struct StrctBMPColBuff
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;

        }BMPColourBuffer;


        // Structure Definition: data in the file header, used to identify bmp
        // SIZE             PURPOSE
        // --------------------------------------------------------------------------------
        // 2                Header field to identify BMP/DIB file (BM ASCII/0x42 0x4D)
        // 4                Size of BMP file in bytes
        // 2                Reserved, see application that creates image
        // 2                Reserved, see application that creates image
        // 4                Offset to starting address of byte of pixel array
        typedef struct StrctBMPFileHeader
        {
            uint16_t nHeaderField;
            uint32_t nFileSiz;
            uint16_t nResCreatorApp1;
            uint16_t nResCreatorApp2;
            uint32_t nBMPOffset;

        }BMPFileHeader;


        // Structure Definition: data in the BITMAPINFOHEADER, provides detail about image
        // SIZE             PURPOSE
        // --------------------------------------------------------------------------------
        // 4                The size of this File Information Header (BITMAPINFOHEADER)
        // 4                Bitmap width in pixels (signed data)
        // 4                Bitmap height in pixels (signed data)
        // 2                The number of colour planes
        // 2                The number of bits per pixel aka colour depth
        // 4                Compression method used, see bitmap documentation
        // 4                The image size, size of raw bitmap data. See case for BI_RGB
        // 4                Horizontal resolution of image (pixel per metre, signed data)
        // 4                Vertical resolution of image (pixel per metre, signed data)
        // 4                Number of colours in the palette
        // 4                Number of important colours. 0 equates to all colours important
        typedef struct StrctBMPInfoHeader
        {
            uint32_t nHeaderSiz;
            int32_t nWidth;
            int32_t nHeight;
            uint16_t nColPlanes;
            uint16_t nBitsPerPix;
            uint32_t nCompressionT;
            uint32_t nBMPByteSiz;
            int32_t nHorzRes;
            int32_t nVertRes;
            uint32_t nCols;
            uint32_t nImportantCols;

        }BMPInfoHeader;


        // Preprocessor Statement END (condense structures)
        #pragma pack(pop)


        // Public Member Functions (Constructor, Destructor, Alphabetical by name)
        Bitmap();
        Bitmap(std::string strBitmapFileToOpen);
        Bitmap(std::string strBitmapFileToOpen, std::string strRGBDataFileToWrite);

        ~Bitmap();

        int export_rgb_data(std::string strRGBDataFileToWrite);

        uint32_t get_file_compression_type();
        std::string get_file_compression_type_readable();
        BMPFileHeader get_file_header();
        uint32_t get_file_size();
        uint16_t get_file_type();
        std::string get_file_type_readable();
        uint16_t get_image_bits_per_pixel();
        uint16_t get_image_colour_planes();
        uint32_t get_image_colours();
        uint32_t get_image_colours_important();
        int32_t get_image_height();
        int32_t get_image_resolution_horizontal();
        int32_t get_image_resolution_vertical();
        uint32_t get_image_size();
        int32_t get_image_width();
        BMPInfoHeader get_info_header();
        EnmBitmapErrors get_last_error();
        uint32_t get_pixel_array_start_address();
        std::vector<BMPColourBuffer> get_pixel_array();

        bool is_supported_bit_depth();

        int load_bitmap_file(std::string strFileToLoad);


    private:
        // Private Member Constants (Alphabetical by type)
        const std::string strCompressionNone = "BI_RGB";
        const std::string strCompressionRLE8 = "BI_RLE8";
        const std::string strCompressionRLE4 = "BI_RLE4";
        const std::string strCompressionBFld = "BI_BITFIELDS";
        const std::string strCompressionJPEG = "BI_JPEG";
        const std::string strCompressionPoNG = "BI_PNG";
        const std::string strCompressionAFld = "BI_ALPHABITFIELDS";
        const std::string strCompressionCMYK = "BI_CMYK";
        const std::string strCompressionCMY8 = "BI_CMYKRLE8";
        const std::string strCompressionCMY4 = "BI_CMYKRLE4";
        const uint16_t nBitmapType = 0x4D42;

        // Private Member Variables (Alphabetical by type)
        bool bLoadedBitmap;
        BMPFileHeader fileHeader;
        BMPInfoHeader infoHeader;
        EnmBitmapErrors enmLastError;
        std::ifstream streamFileRead;
        std::vector<BMPColourBuffer> vecColourBuff;

        // Private Member Functions (Alphabetical by name)
        int read_bit_masks();
        int read_colour_table();
        int read_file_header();
        int read_info_header();
        int read_pixel_array();

    };
}



#endif // BITMAP_H
