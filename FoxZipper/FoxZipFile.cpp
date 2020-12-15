#include "FoxZipFile.h"

void FoxZipFile::catStream(std::istream& is, std::ostream& os)
{
    const std::streamsize buff_size = 1 << 16;
    char* buff = new char[buff_size];
    while (true)
    {
        is.read(buff, buff_size);
        std::streamsize cnt = is.gcount();
        if (cnt == 0) break;
        os.write(buff, cnt);
    }
    delete[] buff;
}

void FoxZipFile::decompressFiles(const std::vector<std::string>& fileList, const std::string& outputFile)
{
    //
    // Set up sink ostream
    //
    std::unique_ptr< std::ofstream > ofs_p;
    std::ostream* os_p = &std::cout;
    if (not outputFile.empty())
    {
        ofs_p = std::unique_ptr< std::ofstream >(new strict_fstream::ofstream(outputFile));
        os_p = ofs_p.get();
    }
    //
    // Process files
    //
    for (const auto& f : fileList)
    {
        //
        // If `f` is a file, create a zstr::ifstream, else (it is stdin) create a zstr::istream wrapper
        //
        std::unique_ptr< std::istream > is_p =
            (f != "-"
                ? std::unique_ptr< std::istream >(new zstr::ifstream(f))
                : std::unique_ptr< std::istream >(new zstr::istream(std::cin)));
        //
        // Cat stream
        //
        catStream(*is_p, *os_p);
    }
}

void FoxZipFile::compressFiles(const std::vector<std::string>& fileList, const std::string& outputFile)
{
    //
    // Set up compression sink ostream
    //
    std::unique_ptr< std::ostream > os_p =
        (not outputFile.empty()
            ? std::unique_ptr< std::ostream >(new zstr::ofstream(outputFile))
            : std::unique_ptr< std::ostream >(new zstr::ostream(std::cout)));
    //
    // Process files
    //
    for (const auto& f : fileList)
    {
        //
        // If `f` is a file, create an ifstream, else read stdin
        //
        std::unique_ptr< std::ifstream > ifs_p;
        std::istream* is_p = &std::cin;
        if (f != "-")
        {
            ifs_p = std::unique_ptr< std::ifstream >(new strict_fstream::ifstream(f));
            is_p = ifs_p.get();
        }
        //
        // Cat stream
        //
        catStream(*is_p, *os_p);
    }
}
