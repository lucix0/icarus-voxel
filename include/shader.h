#pragma once
#include <string>

class Shader {
private:
	unsigned int m_handle;
public:
	Shader();

	void load_from_file(const std::string& vertex_path, const std::string& fragment_path);

	unsigned int get_handle();
};