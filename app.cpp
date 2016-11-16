#include <iostream>
#include "mapped_file.hpp"

int main() {
    Hardwater::MappedFile f("Makefile", "r");
    const Hardwater::MappedFile &fr = f;
    for(uint8_t i: fr) {
        std::cout << static_cast<char>(i);
    }
    return 0;
}
