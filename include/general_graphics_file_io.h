#ifndef  XSP_GENERAL_GRAPHICS_FILE_IO_H
#define XSP_GENERAL_GRAPHICS_FILE_IO_H

#include "general_graphics_file_format.h"

namespace general_graphics_file_ns {

    template<typename REAL>
    void write_general_graphics_file(const GeneralGraphicContent<REAL>& content, const std::string& filepath);

    template<typename REAL>
    bool read_general_graphics_file(const std::string& filepath, GeneralGraphicContent<REAL>& content);

}

#endif