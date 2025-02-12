#include <iostream>
#include <cstdlib>
#include <string>

#ifdef _WIN32
#include <windows.h>  // For GetCurrentDirectory (Windows)
#define COMPILER_PATH "g++"  // Use system-installed g++ on Windows if it's available
#define EXEC_NAME "temp.exe"
#define DELETE_CMD "del temp.exe"
#else
#include <unistd.h>  // For getcwd (Linux/macOS)
#define COMPILER_PATH "clang++"  // Use system-installed clang++ on Linux/macOS
#define EXEC_NAME "./temp"
#define DELETE_CMD "rm temp"
#endif

// Platform-specific code to get current working directory
std::string getCurrentPath() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);
    return std::string(buffer);
#else
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != nullptr) {
        return std::string(buffer);
    } else {
        return "";
    }
#endif
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: smn <file.cpp>" << std::endl;
        return 1;
    }

    std::string cppFile = argv[1];

    // Get the current directory (app path) using platform-specific code
    std::string appPath = getCurrentPath();

    std::string compiler = COMPILER_PATH;
    std::string outputFile = EXEC_NAME;

    // Compile the file
    std::string command = "\"" + compiler + "\" -std=c++17 -o " + outputFile + " " + cppFile;
    int compileStatus = std::system(command.c_str());

    if (compileStatus != 0) {
        std::cerr << "Compilation failed!" << std::endl;
        return 1;
    }

    // Run the compiled file
    std::string runCommand = outputFile;
    int runStatus = std::system(runCommand.c_str());

    // Cleanup compiled file
    std::system(DELETE_CMD);

    return runStatus;
}
