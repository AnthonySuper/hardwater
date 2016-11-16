#include "mapped_file.hpp"

namespace Hardwater {
    MappedFile::MappedFile(FILE *f) :
    fp(f) 
    {
        if(fp == nullptr) {
            throw std::runtime_error("Null file passed");
        }
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        mapped_mem = mmap(nullptr,
                size,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE,
                fileno(fp),
                0);
    }

    MappedFile::MappedFile(std::string s, const char *mode) : 
        MappedFile(fopen(s.c_str(), mode))
    {}

    MappedFile::~MappedFile()
    {
        munmap(mapped_mem, size);
        fclose(fp);
    }
}
