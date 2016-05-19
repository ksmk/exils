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


void glGetShaderSource(GLUint shader, std::string const &shader_str)
{
    GLChar const *shader_source = shader_str.c_str();
    GLInt  const  shader_length = shader_str.size();

    glShaderSource(shader, 1, &shader_source, &shader_length);
}

void load_shader(GLUint shader_obj, Char* const shader_filename)
{
    glGetShaderSource(shader_obj, get_file_contents(shader_filename) );
}

void Shader::Load()
{
    /*

    GLUint shader;

    programs[] = glCreateProgram();
        shader = glCreateShader(GL_VERTEX_SHADER);
        load_shader(shader, "");
        glCompileShader(shader);
        glAttachShader(programs[], shader);
    glLinkProgram(programs[]);

    */
}

void Shader::Activate(Int pid)
{
    glUseProgram(programs[pid]);
    current_pid = pid;
}

void Shader::Deactivate()
{
    glUseProgram(0);
}