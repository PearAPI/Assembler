#pragma once

#include <string>

static inline std::string formatErrorMessage(const std::string& message, size_t line = 0, const std::string& errorType = "Syntax-Error") {
    std::string templateStr = "[\033[31m" + errorType + "\033[0m] "; // Mit kleinem Farbcode für rotes Highlight im Terminal
    
    if (line > 0) {
        templateStr += "on line " + std::to_string(line) + " | ";
    }
    
    templateStr += message;
    
    return templateStr;
}