#pragma once

#include "Definitions.hh"

#define POOL 5

enum
{
    // GLSL LIST
};

class Shader
{
public:
    Shader();
    ~Shader();

    void Load();
    void Activate(Int pid);
    void Deactivate();

    template<typename T>
    void SetUniform(Char* uniform, T value)
    {
        GLInt location = glGetUniformLocation(programs[current_pid], uniform);
        if (location == -1) return;
        if (typeid(T) == typeid(Int)) glUniform1i(location, value);
        else glUniform1f(location, value);
    }

    GLInt GetLocation(Char* uniform)
    {
        return glGetUniformLocation(programs[current_pid], uniform);
    }
private:
    GLUint programs[POOL];
    Int current_pid;
};