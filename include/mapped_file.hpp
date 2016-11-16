#ifndef _HARDWATER_MAPPED_FILE_HPP
#define _HARDWATER_MAPPED_FILE_HPP
#include <sys/mman.h>
#include <stdlib.h>
#include <cstdio>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <sys/stat.h>

namespace Hardwater {
    class MappedFile {
    public:
        using iterator = uint8_t*;
        using const_iterator = uint8_t const *;
        MappedFile& operator=(const MappedFile&) = delete;
        MappedFile(const MappedFile&) = delete;
        MappedFile(std::string fname, size_t size = 0);
        inline size_t getSize() const { return size; }

        inline uint8_t getByte(size_t offset) {
            if(offset > size) {
                throw std::out_of_range("Overflow on buffer");
            }
            return *(static_cast<uint8_t *>(mapped_mem) + offset); 
        }

        inline iterator begin() {
            return static_cast<iterator>(mapped_mem);
        }

        inline iterator end() {
            return static_cast<iterator>(mapped_mem) + size;
        }

        inline const_iterator begin() const {
            return static_cast<const_iterator>(mapped_mem);
        }

        inline const_iterator end() const {
            return static_cast<const_iterator>(mapped_mem);
        }

        ~MappedFile();
    protected:
       FILE *fp;
       void *mapped_mem;
       size_t size;
    };
}
#endif
