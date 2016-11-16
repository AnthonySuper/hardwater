#include "chunk.hpp"

namespace Hardwater {
    Chunk::Chunk(const FragmentHash& hash) :
    hash(hash), beginItr(nullptr), endItr(nullptr)
    {
        
    }
    
    Chunk::Chunk(MappedFile::iterator begin,
                 MappedFile::iterator end) :
    hash(begin, end), beginItr(begin), endItr(end)
    {
        
    }
    
    void Chunk::addFile(MappedFile::iterator begin, MappedFile::iterator end)
    {
        beginItr = begin;
        endItr = end;
    }
    
    bool Chunk::checkValidity() const {
        checkFile();
        FragmentHash tmp(beginItr, endItr);
        return tmp == hash;
    }
    
    
    
    void Chunk::writeOut(const std::vector<uint8_t>& bytes) {
        checkFile();
        if(bytes.size() < endItr - beginItr) {
            throw std::runtime_error("Chunk is wrong size!");
        }
        std::copy(bytes.begin(), bytes.end(), beginItr);
    }
    
    void Chunk::checkFile() const {
        if(beginItr == nullptr || endItr == nullptr) {
            throw std::runtime_error("Chunk is not associated with a file");
        }
    }
    
    std::vector<uint8_t> Chunk::getData() const {
        return {begin(), end()};
    }
}
