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
        // Since we're using GBK encoding font, we need to handle encoding conversion
        // First check if it's already valid GBK, if not try to convert from UTF-8
        if (!IsUTF8(filename)) {
            // Assume it's already in GBK encoding
            return filename;
        }
        
        // Convert UTF-8 to GBK for Chinese characters
        return UTF8ToGBK(filename);
    }
    
    std::string UTF8ToGBK(const std::string& utf8_str) {
        // Simple UTF-8 to GBK conversion for common Chinese characters
        // This handles basic conversion for PSP file browser display
        std::string result;
        size_t i = 0;
        
        while (i < utf8_str.length()) {
            unsigned char c = utf8_str[i];
            
            // ASCII characters (0-127) remain unchanged
            if (c <= 0x7F) {
                result += c;
                i++;
                continue;
            }
            
            // Handle UTF-8 multi-byte sequences
            if ((c & 0xE0) == 0xC0 && i + 1 < utf8_str.length()) {  // 2-byte UTF-8
                unsigned char c2 = utf8_str[i + 1];
                unsigned int code_point = ((c & 0x1F) << 6) | (c2 & 0x3F);
                
                // Basic conversion for common Chinese characters
                // This is a simplified approach - for real use, you'd need a complete conversion table
                if (code_point >= 0x4E00 && code_point <= 0x9FFF) {
                    // CJK Unified Ideographs - use a simple mapping
                    // Note: This is a placeholder - proper conversion requires a full conversion table
                    result += "??";  // Display question marks for Chinese characters
                } else {
                    // Other UTF-8 characters
                    result += '?';  // Replace with question mark
                }
                i += 2;
            }
            else if ((c & 0xF0) == 0xE0 && i + 2 < utf8_str.length()) {  // 3-byte UTF-8
                // For Chinese characters in 3-byte UTF-8
                result += "??";  // Placeholder for Chinese characters
                i += 3;
            }
            else {
                // Invalid UTF-8 sequence
                result += '?';
                i++;
            }
        }
        
        return result;
    }
}