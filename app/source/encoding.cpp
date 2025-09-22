#include "encoding.h"
#include "log.h"

namespace Encoding {
    bool IsUTF8(const std::string& str) {
        size_t i = 0;
        while (i < str.length()) {
            unsigned char c = str[i];
            
            // ASCII character (0-127)
            if (c <= 0x7F) {
                i++;
                continue;
            }
            
            // Multi-byte sequence
            if ((c & 0xE0) == 0xC0) {  // 2-byte sequence
                if (i + 1 >= str.length() || (str[i + 1] & 0xC0) != 0x80)
                    return false;
                i += 2;
            }
            else if ((c & 0xF0) == 0xE0) {  // 3-byte sequence
                if (i + 2 >= str.length() || (str[i + 1] & 0xC0) != 0x80 || (str[i + 2] & 0xC0) != 0x80)
                    return false;
                i += 3;
            }
            else if ((c & 0xF8) == 0xF0) {  // 4-byte sequence
                if (i + 3 >= str.length() || (str[i + 1] & 0xC0) != 0x80 || 
                    (str[i + 2] & 0xC0) != 0x80 || (str[i + 3] & 0xC0) != 0x80)
                    return false;
                i += 4;
            }
            else {
                return false;  // Invalid UTF-8 start byte
            }
        }
        return true;
    }
    
    std::string ConvertFileName(const std::string& filename) {
        // Since we're using GBK encoding font, return the filename as-is
        // The font will handle the GBK encoded characters
        return filename;
    }
    
    std::string UTF8ToGBK(const std::string& utf8_str) {
        // For PSP, since we're using GBK encoding font, we assume the input
        // is already in the correct encoding for display
        // This function is mainly for API compatibility
        return utf8_str;
    }
}