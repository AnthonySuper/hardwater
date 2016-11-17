#ifndef _HARDWATER_SOLUTION_HPP
#define _HARDWATER_SOLUTION_HPP

#include <openssl/rsa.h>
#include <memory>
#include <string>


namespace Hardwater {
    class Solution {
        using string = std::string;
    public:
        Solution(string privatePath, string publicPath);
    private:
        std::shared_ptr<RSA> ptr;
    };
}
#endif
