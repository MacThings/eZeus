#ifndef EGAMEDIR_H
#define EGAMEDIR_H

#include <string>

class eGameDir {
public:
    static void initialize();
    static std::string path(const std::string& path);
    static std::string settingsPath();
    static std::string numbersPath();
    static std::string iBinaryPath();
    static std::string i15BinaryPath();
    static std::string i30BinaryPath();
    static std::string i45BinaryPath();
    static std::string i60BinaryPath();
    static std::string exeDir();
    static std::string adventuresDir();
    static std::string saveDir();
    static std::string texturesDir();
private:
    static std::string sPath;
};

#endif // EGAMEDIR_H
