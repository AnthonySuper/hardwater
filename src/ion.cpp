#include "ion.hpp"

namespace Hardwater {
    
    static size_t determineChunkNumber(size_t size)
    {
        constexpr size_t kilobyte = 1024;
        constexpr size_t megabyte = 1024 * kilobyte;
        constexpr size_t gigabyte = 1024 * megabyte;
        if(size < kilobyte) {
            return 1;
        }
        else if(size < megabyte) {
            return 128;
        }
        else if(size < gigabyte) {
            return 1024;
        }
        else {
            return 2048;
        }
    }
    
    static uint64_t slurpSize(std::vector<uint8_t>::iterator &it)
    {
        // Maybe not stands
        uint64_t ret;
        ret = *(reinterpret_cast<uint64_t*>(&*it));
        it += 8;
        return ret;
    }
    
    static uint32_t slurpSmallerSize(std::vector<uint8_t>::iterator &it)
    {
        uint32_t ret;
        ret = *(reinterpret_cast<uint32_t*>(&*it));
        it += 4;
        return ret;
    }
    
    static uint16_t slurpSmallestSize(std::vector<uint8_t>::iterator &it)
    {
        uint16_t ret;
        ret = *(reinterpret_cast<uint16_t*>(&*it));
        it += 2;
        return ret;
    }
    
    Ion Ion::fromBuff(std::vector<uint8_t> &buff, Key &pub)
    {
        if(pub.isPrivate()) {
            throw std::invalid_argument("Cannot verify with a private key");
        }
        const char *head = "NMTT";
        auto itr = buff.begin();
        if(! std::equal(itr,
                        itr, head)) {
            throw std::runtime_error("This isn't an IonFile");
        }
        itr += 4;
        if(*itr != 1) {
            throw std::runtime_error("File is 2advanced4me");
        }
        itr += 1;
        uint8_t keySize = *itr;
        itr++;
        // Slurp encrypted ion key
        EncryptedIonKey ik(itr, itr + keySize);
        itr += keySize;
        size_t fsize = slurpSize(itr);
        FragmentHash h(itr);
        size_t strLen = slurpSmallerSize(itr);
        std::vector<uint8_t> encryptedFileName(itr, itr + strLen);
        itr += strLen;
        size_t timestamp = slurpSize(itr);
        std::vector<Chunk> chunks;
        size_t numChunks = slurpSmallestSize(itr);
        for(int i = 0; i < numChunks; i++) {
            chunks.emplace_back(Chunk::read(itr));
        }
        
    }
    
    Ion Ion::generate(MappedFile &mf,
                       Key &pub)
    {
        if(pub.isPrivate()) {
            throw std::runtime_error("Type mismatch, public key is private");
        }
        FragmentHash hash(mf.begin(), mf.end());
        IonKey ik;
        auto fn = mf.getFileName();
        auto efn = ik.encrypt(fn);
        EncryptedIonKey ek(ik, pub);
        size_t numChunks = determineChunkNumber(mf.getSize());
        size_t chunkSize = mf.getSize() / numChunks;
        std::vector<Chunk> chunks;
        for(size_t i = 0; i < numChunks; ++i) {
            MappedFile::iterator begin = mf.begin() + (i * chunkSize);
            MappedFile::iterator end = (i == numChunks - 1) ?
                mf.end() : mf.begin() + ((i + 1) * chunkSize);
            auto c = Chunk::generate(begin,
                                     end,
                                     ik,
                                     i);
            chunks.push_back(c);
        }
        std::random_shuffle(chunks.begin(), chunks.end(), Util::RNG());
        return {ek, efn, mf.getSize(), numChunks, hash, std::move(chunks)};
    }
    
    Ion::Ion(EncryptedIonKey &ik,
             std::vector<uint8_t> fn,
             size_t fs,
             size_t nc,
             FragmentHash overall,
             std::vector<Chunk>&& chunks)
    : ionKey(ik),
        encryptedFileName(fn),
        hash(overall),
        fileSize(fs),
        numChunks(nc),
        chunks(chunks)
    {
        
    }
    
    // TODO: Actually sign the code
    std::vector<uint8_t> Ion::dissolve(Hardwater::Key &sign) {
        if(sign.isPublic()) {
            throw std::runtime_error("Type mismatch, privage key is public");
        }
        std::vector<uint8_t> buff;
        // Ion heading (32 bits total)
        buff.emplace_back('N');
        buff.emplace_back('M');
        buff.emplace_back('H');
        buff.emplace_back('W');
        // Protocol version (8 bits)
        auto pf = protocolVersion;
        buff.push_back(pf);
        // Encrypted Ion key length (8 bits)
        auto eik = ionKey.getEncrypted();
        if(eik.size() < std::numeric_limits<uint8_t>::max()) {
            throw std::runtime_error("Cannot store size of eik");
        }
        buff.push_back(eik.size());
        // Encrypted Ion Key (variable bytes)
        buff.insert(buff.end(), eik.begin(), eik.end());
        // File Size (64 bits)
        uint64_t size = fileSize;
        std::array<uint8_t, 8> fileSizeBuff;
        *(reinterpret_cast<uint64_t*>(fileSizeBuff.data())) = size;
        buff.insert(buff.begin(),
                    fileSizeBuff.begin(),
                    fileSizeBuff.end());
        // Hash (256 bits)
        const auto& h = hash.getHash();
        buff.insert(buff.end(), h.begin(), h.end());
        // numChunks (16 bits)
        std::array<uint8_t, 2> ncBuff;
        *reinterpret_cast<uint16_t*>(ncBuff.data()) = numChunks;
        // Encrypted chunks
        for(auto&c : chunks)
        {
            c.writeTo(buff);
        }
        for(int i = 0; i < 4; ++i) {
            buff.emplace_back(0);
        }
        buff.emplace_back(0);
        // TODO: Add stuff
        return buff;
    }
    
    size_t Ion::chunkSize()
    {
        if(fileSize == 0 || numChunks == 0) {
            throw std::runtime_error("Cannot determine chunk size!");
        }
        return fileSize / numChunks;
    }
}
