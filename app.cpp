#include <iostream>
#include "mapped_file.hpp"
#include "ion.hpp"
#include <iostream>

int main(int argc, const char ** argv) {
    using std::cerr;
    using std::cout;
    using std::endl;
    if(argc < 2) {
        cerr << "Need a file name argument" << endl;
        return -1;
    }
    auto m = std::make_unique<Hardwater::MappedFile>(argv[1]);
    Hardwater::Ion i(std::move(m));
    i.generate();
    cout << "File has " << i.getNumChunks() << " chunks with a size of " << i.getChunkSize() << "bytes" << endl;
    cout << "Overall hash: " << i.getOverallHash().getDigest() << endl;
    auto c = i.getChunks();
    for(int i = 0; i < c.size(); ++i) {
        cout << "Chunk " << i << ": " << c.at(i).getDigest() << endl;
    }
    return 0;
}
