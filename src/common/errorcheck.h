#ifndef MEX_COMMON_ERROR_CHECK_H
#define MEX_COMMON_ERROR_CHECK_H

#include <errno.h>
#include <iostream>

namespace mex {

namespace common {

#define CHECK_ERROR(error) \
    CheckErrorno(__PRETTY_FUNCTION__, __FILE__, __LINE__, (error))

inline void CheckErrorno(const char *function,
                         const char *file,
                         int line,
                         int error) {
    if (error) {
        const char *msg = stderror(error);
        std::cerr << "[ERROR] Function: " << function << " ("
            << file << ": " << line << ") " << msg << std::endl;
        abort();
    }
}

} // namespace common

} // namespace mex

#endif // MEX_COMMON_ERROR_CHECK_H
