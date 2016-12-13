#ifndef _HARDWATER_MAPPED_FILE_HPP
#define _HARDWATER_MAPPED_FILE_HPP
#include <sys/mman.h>
#include <stdlib.h>
#include <cstdio>
#include <string>
#include <cstdint>
#include <stdexcept>
#define _POISX_C_SOURCE
#include <sys/stat.h>

namespace Hardwater {
    /**
     * @breif a memory-mapped file
     *
     * Provides a nice abstracton over mmap, which allows us to use iterators
     * over the contents of a file.
     */
    class MappedFile {
        
    public:
        /**
         * Iterator type
         */
        using iterator = uint8_t*;
        
        /**
         * Constant iterator type
         */
        using const_iterator = uint8_t const *;
        
        MappedFile& operator=(const MappedFile&) = delete;
        
        MappedFile(const MappedFile&) = delete;
        
        /**
         * Initialize a filename with a name
         * @param fname the file name to open
         * @param size the optional size to allocate for the file if it does not currently exist
         */
        MappedFile(std::string fname, size_t size = 0);
        
        /**
         * Get the overall file size
         */
        inline size_t getSize() const { return size; }
        
        /**
         * Get a specific byte
         */
        inline uint8_t getByte(size_t offset) {
            if(offset > size) {
                throw std::out_of_range("Overflow on buffer");
            }
            return *(static_cast<uint8_t *>(mapped_mem) + offset); 
        }
        
        /**
         * @brief Get an iterator to the start of the file
         *
         */
        inline iterator begin() {
            return static_cast<iterator>(mapped_mem);
        }

        /**
         * @brief get an iterator to the end of the file
         */
        inline iterator end() {
            return static_cast<iterator>(mapped_mem) + size;
        }

        /**
         * @brief get a constant iterator to the start of the file
         */
        inline const_iterator begin() const {
            return static_cast<const_iterator>(mapped_mem);
        }

        /**
         * @brief get a constant iterator to the end of the file
         */
        inline const_iterator end() const {
            return static_cast<const_iterator>(mapped_mem);
        }
        
        /**
         * @brief get the time the file was last modified at
         */
        inline time_t getTimestamp() const {
            return timestamp;
        }
        
        /**
         * @brief get the name of the file
         */
        inline const std::string& getFileName() const {
            return fileName;
        }
        

        ~MappedFile();
    protected:
        FILE *fp;
        void *mapped_mem;
        size_t size;
        time_t timestamp;
        const std::string fileName;
    };
}
#endif
