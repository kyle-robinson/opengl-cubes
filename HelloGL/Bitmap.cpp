// PROJECT:										Bitmap Class - Fundamentals of Game and Graphical System Development (COSE40630)
// FILENAME:									Bitmap.cpp
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
//                                              Provided are three constructors:
//                                              1: Basic constructor, simply sets up the class and requires no parameters. A valid call to load_bitmap_file can
//                                              then be made after construction
//
//                                              2: Constructor with load parameter, this automatically loads a file given by the string parameter, this must
//                                              include the file path, name and extension to load correctly. Use the get_last_error to ensure that the load was successful
//
//                                              3: Convenience constructor, automatically loads file given by first parameter and exports the rgb pixel array to a file
//                                              at the second parameter. Use the get_last_error to ensure that the load and export were successful
//
//                                              Exporting, various accessor functions including get_pixel_array should be called after a successful load_bitmap_file
//                                              check the error return values provided by public function interface
//
//                                              load_bitmap_file can be called after an instance of the class has been constructed. Where the file is not compatible
//                                              the headers may still be accessible for the user to read data but the pixel array will not be available for export or use
//
//                                              It is the responsibility of the user to provide a valid bitmap file (24 bit BITMAPINFOHEADER type, uncompressed RGB bottom up format)
//                                              and to use error returns


// Change Log:


// Included Header Files (Alphabetical STL then followed by custom)
#include "Bitmap.h"


// Namespace
using namespace FOGGS;


// FUNCTION:									constructor
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Set up class instance. Basic constructor. A call to load_bitmap_file will be required after this
// PARAMETERS:                                  NA
// RETURN:                                      NA
// KEY VARIABLES:                               Member boolean | enumeration | BITMAPFILEHEADER | BITMAPINFOHEADER
// KEY FUNCTIONS AND LIBRARIES:
// MODIFICATIONS AND NOTES:
Bitmap::Bitmap()
{
    this->bLoadedBitmap = false;
    this->enmLastError = EnmBitmapErrors::NOERR;
    this->fileHeader = BMPFileHeader();
    this->infoHeader = BMPInfoHeader();
}


// FUNCTION:									constructor (overload)
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Set up class instance. Automatically loads a bitmap file, allowing public interface functions to be
//                                              used on the loaded data
// PARAMETERS:                                  std::string strBitmapFileToOpen - the file (path,name,extension) to load
// RETURN:                                      NA
// KEY VARIABLES:
// KEY FUNCTIONS AND LIBRARIES:                 Basic constructor and load_bitmap_file
// MODIFICATIONS AND NOTES:                     Use get_last_error to check for any error loading the bitmap file
Bitmap::Bitmap(std::string strBitmapFileToOpen)
{
    Bitmap();
    load_bitmap_file(strBitmapFileToOpen);
}


// FUNCTION:									constructor (overload)
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Set up class instance. Automatically load a bitmap file and export the rgb pixel array to provided
//                                              file location given as parameter
// PARAMETERS:                                  std::string strBitmapFileToOpen - the file (path,name,extension) to load for use |
//                                              std::string strRGBDataFileToWrite - the file (path,name,extension) where the
//                                              rgb pixel array will be written
// RETURN:                                      NA
// KEY VARIABLES:
// KEY FUNCTIONS AND LIBRARIES:                 Basic constructor, export_pixel_array and load_bitmap_file
// MODIFICATIONS AND NOTES:                     Use get_last_error to check for any error loading and exporting bitmap data
Bitmap::Bitmap(std::string strBitmapFileToOpen, std::string strRGBDataFileToWrite)
{
    Bitmap();

    if(static_cast<EnmBitmapErrors>(load_bitmap_file(strBitmapFileToOpen)) == EnmBitmapErrors::NOERR)
    {
        export_rgb_data(strRGBDataFileToWrite);
    }
}


// FUNCTION:									destructor
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Clean up the class instance
// PARAMETERS:                                  NA
// RETURN:                                      NA
// KEY VARIABLES:                               Member file stream
// KEY FUNCTIONS AND LIBRARIES:                 fstream
// MODIFICATIONS AND NOTES:
Bitmap::~Bitmap()
{
    if(this->streamFileRead.is_open())
    {
        this->streamFileRead.close();
    }
}


// FUNCTION:									export_rgb_data
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.1
// DATE:                                        10/02/2020 | V1.1 17/02/2020
// PURPOSE:                                     Write the pixel array out to file at location given by string parameter
// PARAMETERS:                                  std::string File path, name, extension to write to
// RETURN:                                      int 0 - success | 7 - Error opening to write | 8 - Error during write |
//                                              11 - No compatible bitmap file has been loaded, pixel array is empty
// KEY VARIABLES:                               File stream | Member vector containing data | int nRetVal used to return error state of function
// KEY FUNCTIONS AND LIBRARIES:                 fstream
// MODIFICATIONS AND NOTES:                     Will create or overwrite file. Developer should ensure that load_bitmap_file is called prior to
//                                              making a call to this function to avoid an error
//
//                                              V1.1 - Modified to ensure output is binary type, .write replaces streaming op - LDE
int Bitmap::export_rgb_data(std::string strRGBDataFileToWrite)
{
    // Local Variables
    unsigned int nCount = 0;
    int nRetVal = static_cast<int>(EnmBitmapErrors::NOERR);
    std::ofstream streamFileWrite;

    if(!this->vecColourBuff.empty())
    {
        // Open the file for output
        streamFileWrite.open(strRGBDataFileToWrite, std::ios::binary);

        if(streamFileWrite.is_open() && streamFileWrite.good())
        {
            try
            {
                for(nCount = 0; nCount < this->vecColourBuff.size(); nCount++)
                {
                    streamFileWrite.write(reinterpret_cast<char *>(&this->vecColourBuff.at(nCount)), sizeof(FOGGS::Bitmap::BMPColourBuffer));
                }
            }
            catch (std::ofstream::failure exceptWriteFile)
            {
                nRetVal = static_cast<int>(EnmBitmapErrors::WRITERAWDATA);
            }
        }
        else
        {
            nRetVal = static_cast<int>(EnmBitmapErrors::NOOUTPUTFILE);
        }

        // Cleanup
        streamFileWrite.close();
    }
    else
    {
        nRetVal = static_cast<int>(EnmBitmapErrors::NOLOADED);
    }

    this->enmLastError = static_cast<EnmBitmapErrors>(nRetVal);

    return nRetVal;
}


// FUNCTION:									get_file_compression_type
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Return numeric value for compression type
// PARAMETERS:                                  NA
// RETURN:                                      unsigned int 32 width for compression type.
// KEY VARIABLES:                               Member structure for BITMAPINFOHEADER
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     Compression type is detailed in Bitmap standard documentation
uint32_t Bitmap::get_file_compression_type()
{
    return this->infoHeader.nCompressionT;
}


// FUNCTION:									get_file_compression_type_readable
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Return a string identifier for the compression type used
// PARAMETERS:                                  NA
// RETURN:                                      String for compression type. If unknown, behaviour is undefined
// KEY VARIABLES:                               Member structure for BITMAPINFOHEADER | std::string strRet to return the readable value
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap | string
// MODIFICATIONS AND NOTES:                     Requires the compression type to be of correct type, else behaviour is undefined
//                                              See the Bitmap standard documentation for compression types used
std::string Bitmap::get_file_compression_type_readable()
{
    // Local Variable
    std::string strRet;

    switch(static_cast<EnmCompressionMethods>(this->infoHeader.nCompressionT))
    {
        case EnmCompressionMethods::BI_RGB:
        strRet = this->strCompressionNone;
        break;

        case EnmCompressionMethods::BI_RLE8:
        strRet = this->strCompressionRLE8;
        break;

        case EnmCompressionMethods::BI_RLE4:
        strRet = this->strCompressionRLE4;
        break;

        case EnmCompressionMethods::BI_BITFIELDS:
        strRet = this->strCompressionBFld;
        break;

        case EnmCompressionMethods::BI_JPEG:
        strRet = this->strCompressionJPEG;
        break;

        case EnmCompressionMethods::BI_PNG:
        strRet = this->strCompressionPoNG;
        break;

        case EnmCompressionMethods::BI_ALPHABITFIELDS:
        strRet = this->strCompressionAFld;
        break;

        case EnmCompressionMethods::BI_CMYK:
        strRet = this->strCompressionCMYK;
        break;

        case EnmCompressionMethods::BI_CMYKRLE8:
        strRet = this->strCompressionCMY8;
        break;

        case EnmCompressionMethods::BI_CMYKRLE4:
        strRet = this->strCompressionCMY4;
        break;
    }

    return strRet;
}


// FUNCTION:									get_file_header
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns a populated structure containing all information from the BITMAPFILEHEADER
// PARAMETERS:                                  NA
// RETURN:                                      Populated structure matching windows definition of BITMAPFILEHEADER
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     If no bitmap is loaded when this function is called, empty structure is returned safely
//
//                                              Possibly provide a function that populates a BITMAPFILEHEADER via reference/pointer rather than return
//                                              to minimise copy, file header is only 14 bytes
Bitmap::BMPFileHeader Bitmap::get_file_header()
{
   return this->fileHeader;
}


// FUNCTION:									get_file_size
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for file size (headers + pixel array)
// PARAMETERS:                                  NA
// RETURN:                                      unsigned integer width 32 representing file size
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     File size in bytes
uint32_t Bitmap::get_file_size()
{
    return this->fileHeader.nFileSiz;
}


// FUNCTION:									get_file_type
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns the file type code as a numeric value,
//                                              handled internally as hex but uint16_t does not specify base
// PARAMETERS:                                  NA
// RETURN:                                      uint16_t code representing file type. uint16_t used to guarentee width. Could be an unsigned short
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     If no bitmap is loaded when this function is called, zero is returned safely
//                                              See Bitmap standard documentation for possible values. Expected value is 0x4D42 (BM)
uint16_t Bitmap::get_file_type()
{
    return this->fileHeader.nHeaderField;
}


// FUNCTION:									get_file_type_readable
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns the file type code. For application to work this should be BM. Returned in human readable form
// PARAMETERS:                                  NA
// RETURN:                                      std::string containing two character code identifying the file type
// KEY VARIABLES:                               std::string strRet populated using push_back member function
// KEY FUNCTIONS AND LIBRARIES:                 string | Bitmap
// MODIFICATIONS AND NOTES:                     To convert from unsigned to signed, static_cast is used
//                                              Bitwise manipulation and explicit truncation used to convert from unsigned 16 bit number to character
//
//                                              If no bitmap is loaded when this function is called, an empty string is returned safely
//
//                                              It is the responsibility of the developer using this library to check that a valid bitmap is loaded or
//                                              to use the convenience constructor
std::string Bitmap::get_file_type_readable()
{
    // Local Variable
    std::string strRet;

    strRet.push_back(static_cast<char>(this->fileHeader.nHeaderField & 0xFF));
    strRet.push_back(static_cast<char>((this->fileHeader.nHeaderField >> 8) & 0xFF));

    return strRet;
}


// FUNCTION:									get_image_bits_per_pixel
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for bits per pixel
// PARAMETERS:                                  NA
// RETURN:                                      unsigned integer width 16 representing bits per pixel
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     Also known as bit depth or colour depth of the image
uint16_t Bitmap::get_image_bits_per_pixel()
{
    return this->infoHeader.nBitsPerPix;
}


// FUNCTION:									get_image_colour_planes
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for colour planes
// PARAMETERS:                                  NA
// RETURN:                                      unsigned integer width 16 representing colour planes present in image
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     For the purpose of Windows style Bitmap, almost certainly 1 see Bitmap documentation
uint16_t Bitmap::get_image_colour_planes()
{
    return this->infoHeader.nColPlanes;
}


// FUNCTION:									get_image_colours
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for colours present in image
// PARAMETERS:                                  NA
// RETURN:                                      unsigned integer width 32 representing colours in the image
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:
uint32_t Bitmap::get_image_colours()
{
    return this->infoHeader.nCols;
}


// FUNCTION:									get_image_colours_important
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for colours considered important in the image
// PARAMETERS:                                  NA
// RETURN:                                      unsigned integer width 32 representing colours considered important in the image
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     The function will return zero in the event that all colours are considered important OR the value is unknown
uint32_t Bitmap::get_image_colours_important()
{
    return this->infoHeader.nImportantCols;
}


// FUNCTION:									get_image_height
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for the image height
// PARAMETERS:                                  NA
// RETURN:                                      signed integer width 32 representing image height
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     Height returned in pixels. If positive, in uncompressed RGB Bitmaps, pixel array starts bottom left
int32_t Bitmap::get_image_height()
{
    return this->infoHeader.nHeight;
}


// FUNCTION:									get_image_resolution_horizontal
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for the horizontal image resolution
// PARAMETERS:                                  NA
// RETURN:                                      signed integer width 32 representing horizontal image resolution
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     Measured in pixels per meter
int32_t Bitmap::get_image_resolution_horizontal()
{
    return this->infoHeader.nHorzRes;
}


// FUNCTION:									get_image_resolution_vertical
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for the vertical image resolution
// PARAMETERS:                                  NA
// RETURN:                                      signed integer width 32 representing vertical image resolution
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     Measured in pixels per meter
int32_t Bitmap::get_image_resolution_vertical()
{
    return this->infoHeader.nVertRes;
}


// FUNCTION:									get_image_size
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for the size of the bitmap data
// PARAMETERS:                                  NA
// RETURN:                                      unsigned integer width 32 representing size of the bitmap data
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     According to standard, if no compression used function can return 0 bytes
uint32_t Bitmap::get_image_size()
{
    return this->infoHeader.nBMPByteSiz;
}


// FUNCTION:									get_image_width
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for the image width
// PARAMETERS:                                  NA
// RETURN:                                      signed integer width 32 representing image width
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     Width measured in pixels, can be used for image stride calculations
int32_t Bitmap::get_image_width()
{
    return this->infoHeader.nWidth;
}


// FUNCTION:									get_info_header
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns populated BITMAPINFOHEADER
// PARAMETERS:                                  NA
// RETURN:                                      BITMAPINFOHEADER populated with data from the bitmap this instance represents
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     If no valid bitmap is loaded, this will safely return an empty structure.
//
//                                              Possibly rework to provide a function populating pointer or reference parameter.
Bitmap::BMPInfoHeader Bitmap::get_info_header()
{
    return this->infoHeader;
}


// FUNCTION:									get_last_error
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        10/02/2020
// PURPOSE:                                     Returns the enumeration for the last error encountered
// PARAMETERS:                                  NA
// RETURN:                                      Enumeration see header file for documentation
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     Intended for use when convenience constructor used, does not clear the last error when read
//                                              a call to the load or export functions will reset this value
//                                              Please use the return values of the public interface to check for error state
Bitmap::EnmBitmapErrors Bitmap::get_last_error()
{
    return this->enmLastError;
}


// FUNCTION:									get_pixel_array_start_address
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns numeric value for offset in bytes to where the pixel array starts
// PARAMETERS:                                  NA
// RETURN:                                      unsigned integer width 32 representing start address in the file for the pixel array
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:
uint32_t Bitmap::get_pixel_array_start_address()
{
    return this->fileHeader.nBMPOffset;
}


// FUNCTION:									get_pixel_array
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns a vector containing structures of RGB data
// PARAMETERS:                                  NA
// RETURN:                                      std::vector containing structures of RGB
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap | vector
// MODIFICATIONS AND NOTES:                     This should be called after a valid Bitmap has been loaded. An empty vector will be returned
//                                              if no valid Bitmap loaded. It is the responsiblity of the user to load the Bitmap correctly
//
//                                              Headers may be available where an incompatible Bitmap is loaded but the pixel array will
//                                              not be loaded
std::vector<Bitmap::BMPColourBuffer> Bitmap::get_pixel_array()
{
    return this->vecColourBuff;
}


// FUNCTION:									is_supported_bit_depth
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Returns boolean value for whether or not the bit depth is supported
// PARAMETERS:                                  NA
// RETURN:                                      Boolean true for supported, false not supported
// KEY VARIABLES:                               NA
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     /*|| this->infoHeader.nBitsPerPix == 32*/
//                                              Currently, this programme only supports 24 bit uncompressed RGB Bitmaps
bool Bitmap::is_supported_bit_depth()
{
    return (this->infoHeader.nBitsPerPix == 24);
}


// FUNCTION:									load_bitmap_file
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Loads a bitmap into memory, separating the used components into data structures
// PARAMETERS:                                  std::string file directory, name and extension.
// RETURN:                                      int error code 0 - success | other values see definition in header file
// KEY VARIABLES:                               ifstream streamFileRead - used to access file | int nRetVal used to return error state
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap | fstream | iostream
// MODIFICATIONS AND NOTES:                     Where the basic constructor is used, this function should be called with valid data
//                                              prior to using the accessor functions or export function for expected output to occur
int Bitmap::load_bitmap_file(std::string strFileToLoad)
{
    // Local Variables
    int nRetVal = static_cast<int>(EnmBitmapErrors::NOERR);

    // Open File in binary mode
    this->streamFileRead.open(strFileToLoad, std::ios::binary);

    if(this->streamFileRead.is_open() && this->streamFileRead.good())
    {
        nRetVal = read_file_header();

        if(static_cast<EnmBitmapErrors>(nRetVal) == EnmBitmapErrors::NOERR)
        {
           nRetVal = read_info_header();

           if(static_cast<EnmBitmapErrors>(nRetVal) == EnmBitmapErrors::NOERR)
           {
                // Optional implementation
                if(static_cast<EnmCompressionMethods>(this->infoHeader.nCompressionT) == EnmCompressionMethods::BI_BITFIELDS)
                {
                   nRetVal = read_bit_masks();
                }

                // Optional implementation
                nRetVal = read_colour_table();

                if(static_cast<EnmBitmapErrors>(nRetVal) == EnmBitmapErrors::NOERR)
                {
                    nRetVal = read_pixel_array();
                }
           }
        }
    }
    else
    {
        nRetVal = static_cast<int>(EnmBitmapErrors::NOINPUTFILE);
    }

    // Cleanup
    this->streamFileRead.close();
    this->enmLastError =  static_cast<EnmBitmapErrors>(nRetVal);

    return nRetVal;
}


// FUNCTION:									read_bit_masks
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Read the optional 12 byte bit masks as identified in the standard
// PARAMETERS:                                  NA
// RETURN:                                      int error code 0 - success
// KEY VARIABLES:
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     NOT IMPLEMENTED
int Bitmap::read_bit_masks()
{
    // Local Variables
    int nRetVal = static_cast<int>(EnmBitmapErrors::NOERR);

    return nRetVal;
}


// FUNCTION:									read_colour_table
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Read the optional colour table (programme does not support bit depths where compulsory)
// PARAMETERS:                                  NA
// RETURN:                                      int error code 0 - success
// KEY VARIABLES:
// KEY FUNCTIONS AND LIBRARIES:                 Bitmap
// MODIFICATIONS AND NOTES:                     NOT IMPLEMENTED
int Bitmap::read_colour_table()
{
    // Local Variables
    int nRetVal = static_cast<int>(EnmBitmapErrors::NOERR);

    return nRetVal;
}


// FUNCTION:									read_file_header
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.1
// DATE:                                        07/02/2020 | 09/02/2020 V1.1
// PURPOSE:                                     Reads the file header, performs actions in expected format order
// PARAMETERS:                                  NA (private members used)
// RETURN:                                      int 0 - success | 3 - file not a BMP | 4 - Error reading header
// KEY VARIABLES:                               int nRetVal used to return error status of function.
// KEY FUNCTIONS AND LIBRARIES:                 iostream | fstream | Bitmap
// MODIFICATIONS AND NOTES:                     Developer must provide an open and valid file stream
//                                              function is intended to be called by load_bitmap which performs this check.
//
//                                              Due to Windows (and a non standard compiler hack) this structure is read
//                                              on separate lines, whereas the second structure is read directly. Windows treats
//                                              the structure that is 14 bytes as 16 bytes (as seen in IDE and calculations). This
//                                              avoids use of a non-standard #pragma declaration. (See pragma pack).
//
//                                              09/02/2020 - Modified to use private member data for class implementation LDE
//
//                                              Must be called in order, does not seek file pointer (before infoheader, prior to options and colour)
int Bitmap::read_file_header()
{
    // Local Variables
    int nRetVal = static_cast<int>(EnmBitmapErrors::NOERR);

    try
    {
        this->streamFileRead.read(reinterpret_cast<char *>(&this->fileHeader.nHeaderField), sizeof(this->fileHeader.nHeaderField));

        // Check for file type, if correct proceed (see notes in header block)
        if(this->fileHeader.nHeaderField == this->nBitmapType)
        {
            this->streamFileRead.read(reinterpret_cast<char *>(&this->fileHeader.nFileSiz), sizeof(this->fileHeader.nFileSiz));
            this->streamFileRead.read(reinterpret_cast<char *>(&this->fileHeader.nResCreatorApp1), sizeof(this->fileHeader.nResCreatorApp1));
            this->streamFileRead.read(reinterpret_cast<char *>(&this->fileHeader.nResCreatorApp2), sizeof(this->fileHeader.nResCreatorApp2));
            this->streamFileRead.read(reinterpret_cast<char *>(&this->fileHeader.nBMPOffset), sizeof(this->fileHeader.nBMPOffset));
        }
        else
        {
            nRetVal = static_cast<int>(EnmBitmapErrors::NOBMP);
        }
    }
    catch (std::ifstream::failure exceptFileRead)
    {
        nRetVal = static_cast<int>(EnmBitmapErrors::READFILEHEAD);
    }

    return nRetVal;
}


// FUNCTION:									read_info_header
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.1
// DATE:                                        07/02/2020 | 09/02/2020 V1.1
// PURPOSE:                                     Reads the info header (BITMAPINFOHEADER), performs actions in expected format order
// PARAMETERS:                                  NA (uses private member data)
// RETURN:                                      int 0 - success | 5 - Error reading header | 10 - Header version is not compatible with this programme
// KEY VARIABLES:                               int nRetVal used to return error status of function.
// KEY FUNCTIONS AND LIBRARIES:                 iostream | fstream | Bitmap
// MODIFICATIONS AND NOTES:                     Developer must provide an open and valid file stream
//                                              function is intended to be called by load_bitmap which performs this check.
//
//                                              09/02/2020 - Modified to be used in class implementation LDE
//
//                                              Must be called in order, does not seek file pointer (after fileheader, prior to options and colour)
int Bitmap::read_info_header()
{
    // Local Variables
    int nRetVal = static_cast<int>(EnmBitmapErrors::NOERR);

    try
    {
        this->streamFileRead.read(reinterpret_cast<char *>(&this->infoHeader), sizeof(BMPInfoHeader));

        if(this->infoHeader.nHeaderSiz != 40)
        {
            nRetVal = static_cast<int>(EnmBitmapErrors::NOHEADERVERS);
        }
    }
    catch (std::ifstream::failure exceptFileRead)
    {
        nRetVal = static_cast<int>(EnmBitmapErrors::READINFOHEAD);
    }

    return nRetVal;
}


// FUNCTION:									read_pixel_array
// COMPANY:										Staffordshire University - School of Computing and Digital Technologies - Games and Visual Effects
// VERSION:										1.0
// DATE:                                        09/02/2020
// PURPOSE:                                     Reads the pixel array of the bitmap file
// PARAMETERS:                                  NA (uses private member data)
// RETURN:                                      int 0 - success | 6 - error reading the pixel array from file | 9 - not a supported bit depth
// KEY VARIABLES:                               int nRetVal used to return error status of function.
// KEY FUNCTIONS AND LIBRARIES:                 iostream | fstream | Bitmap
// MODIFICATIONS AND NOTES:                     Developer must provide an open and valid file stream
//                                              function is intended to be called by load_bitmap which performs this check.
//
//                                              Bitmap stores values bgr format. Accounts for possible padding values.
//
//                                              Testing successful 10/02/2020
int Bitmap::read_pixel_array()
{
    // Local Variables
    BMPColourBuffer buffColour;
    int nColCount = 0, nRowCount = 0;
    int nRowSiz = 0, nPadding = 0;
    int nRetVal = static_cast<int>(EnmBitmapErrors::NOERR);

    if(is_supported_bit_depth())
    {
        // Seek file pointer to pixel array start
        this->streamFileRead.seekg(this->fileHeader.nBMPOffset, this->streamFileRead.beg);

        // Calculate length of a row and padding bytes
        nRowSiz = ((this->infoHeader.nBitsPerPix * this->infoHeader.nWidth) / 32) * 4;
        nPadding = this->infoHeader.nWidth % 4;

        try
        {
            for(nRowCount = 0; nRowCount < this->infoHeader.nHeight; nRowCount++)
            {
                for(nColCount = 0; nColCount < nRowSiz; nColCount += 3)
                {
                    this->streamFileRead.read(reinterpret_cast<char *>(&buffColour.b), sizeof(buffColour.b));
                    this->streamFileRead.read(reinterpret_cast<char *>(&buffColour.g), sizeof(buffColour.g));
                    this->streamFileRead.read(reinterpret_cast<char *>(&buffColour.r), sizeof(buffColour.r));

                    this->vecColourBuff.push_back(buffColour);
                }

                // Advance file past line padding
                this->streamFileRead.seekg(nPadding, this->streamFileRead.cur);
            }
        }
        catch (std::ifstream::failure exceptFileRead)
        {
            nRetVal = static_cast<int>(EnmBitmapErrors::READRAWDATA);
        }
    }
    else
    {
        nRetVal = static_cast<int>(EnmBitmapErrors::UNSUPPORTED);
    }

    return nRetVal;
}
