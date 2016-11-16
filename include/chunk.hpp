#ifndef _HARDWATER_CHUNK_HPP
#define _HARDWATER_CHUNK_HPP
#include "fragment_hash.hpp"
#include "mapped_file.hpp"
#include <vector>

namespace Hardwater {
    class Chunk {
    public:
        Chunk(const FragmentHash& hash);
        Chunk(MappedFile::iterator begin,
              MappedFile::iterator end);
        
        void addFile(MappedFile::iterator begin,
                     MappedFile::iterator end);
        
        bool checkValidity() const;
        
        void writeOut(const std::vector<uint8_t>& bytes);
        
        inline FragmentHash getHash() {
            return hash;
        }
        
        inline const FragmentHash& getHash() const {
            return hash;
        }
        
        inline MappedFile::iterator begin() {
            return beginItr;
        }
        
        inline MappedFile::iterator end() {
            return endItr;
        }
        
        inline MappedFile::const_iterator begin() const {
            return static_cast<MappedFile::const_iterator>(beginItr);
        }
        
        inline MappedFile::const_iterator end() const {
            return static_cast<MappedFile::const_iterator>(endItr);
        }
        
        std::vector<uint8_t> getData() const;
        
    private:
        FragmentHash hash;
        MappedFile::iterator beginItr;
        MappedFile::iterator endItr;
        void checkFile() const;
    };
}
#endif
