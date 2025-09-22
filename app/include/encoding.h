#pragma once

#include <string>

namespace Encoding {
    // Check if a string is valid UTF-8
    bool IsUTF8(const std::string& str);
    
    // Simple UTF-8 to GBK conversion for common Chinese characters
    std::string UTF8ToGBK(const std::string& utf8_str);
    
    // Convert file system encoding to display encoding
    std::string ConvertFileName(const std::string& filename);
}