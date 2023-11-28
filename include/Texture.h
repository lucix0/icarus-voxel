#pragma once
#include <string>

class Texture {
private:
	unsigned int m_handle;
public:
	Texture();

	void load_from_file(const std::string& path);

	unsigned int get_handle();
};
