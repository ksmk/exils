#include "Shader.hh"

Shader::Shader(){}
Shader::~Shader(){}

std::string get_file_contents(const Char* filename)
{
    std::ifstream ifs(filename);
    std::string content
    (   (std::istreambuf_iterator<Char>(ifs))  , 
        (std::istreambuf_iterator<Char>()   )  );
    return content;
}

void Shader::Load() {}

void Shader::Activate(Int pid) {}

void Shader::Deactivate() {}