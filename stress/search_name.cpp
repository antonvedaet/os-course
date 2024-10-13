#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void find_file(const std::string& filename, const fs::path& search_path) {
    bool found = false;
    for (const auto& entry : fs::recursive_directory_iterator(search_path)) {
        if (entry.is_regular_file() && entry.path().filename() == filename) {
            std::cout << entry.path() << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "file not found" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "usage: " << argv[0] << " <filename> <search_path> <n>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    fs::path search_path = argv[2];
    int repeat = std::stoi(argv[3]);

    for (int i = 0; i < repeat; ++i) {
        std::cout << "search... (" << (i + 1) << "/" << repeat << ")" << std::endl;
        find_file(filename, search_path);
    }

    return 0;
}
