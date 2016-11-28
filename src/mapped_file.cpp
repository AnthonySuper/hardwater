#include "mapped_file.hpp"
#include <iostream>

namespace Hardwater {
    
#ifdef __APPLE__
    static time_t getModified(struct stat *s) {
        return s->st_mtimespec.tv_sec;
    }
#else
    static time_t getModified(struct stat *s) {
        return s->st_mtime;
    }
#endif
    
    MappedFile::MappedFile(std::string str, size_t s)
    : fileName(str)
    {
        
        fp = fopen(str.c_str(), "r+");
        if(fp == nullptr) {
            if(errno == ENOENT) {
                fp = fopen(str.c_str(),"w+");
            }
            if(fp == nullptr) {
                throw std::runtime_error("File creation failed!");
            }
        }
        if(s == 0) {
            struct stat st;
            fstat(fileno(fp), &st);
            size = st.st_size;
            timestamp = getModified(&st);
        }
        else {
            size = s;
            // allocate space for the file
            fseek(fp, size, SEEK_SET);
            fputc('\0', fp);
            fseek(fp, 0, SEEK_SET);
        }
        if(size == 0) {
            throw std::invalid_argument("Size of file is zero");
        }
        mapped_mem = mmap(nullptr,
                          size,
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED,
                          fileno(fp),
                          0);
        if(mapped_mem == reinterpret_cast<void *>(-1)) {
            perror("Recieved an error in mmap");
            throw std::runtime_error("MMap failure");
        }

    }

    MappedFile::~MappedFile()
    {
        munmap(mapped_mem, size);
        fclose(fp);
    }
}
