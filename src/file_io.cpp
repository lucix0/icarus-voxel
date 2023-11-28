#include "file_io.h"
#include <fstream>

bool read_text_file_into_string(const std::string& path, std::string& destination) {
    std::ifstream stream;
    stream.open(path, std::ios_base::in);

    stream.seekg(0, std::ios::end);
    int size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    std::string buf = std::string(size, '\0');
    if (stream.is_open()) {
        stream.read(&buf[0], size);
    }
    else {
        return false;
    }

    destination = std::move(buf);

    stream.close();
    return true;
}