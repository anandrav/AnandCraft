#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "GL/glew.h"

// Thin RAII wrapper class around OpenGL shader
class Shader {
public:
    // the program ID
    GLuint ID;

    Shader()
    {
        ID = 0;
    }

    // constructor reads and builds the shader
    Shader(const std::string& vertex_path, const std::string& fragment_path) 
    {
        setup_program(vertex_path, fragment_path);
    }

    Shader(Shader&& other) noexcept
    {
        ID = other.ID;
        // prevent destructor from destroying program
        other.ID = 0;
    }

    Shader& operator=(Shader&& other) noexcept
    {
        std::swap(ID, other.ID);
        return *this;
    }

    ~Shader() 
    {
        glDeleteProgram(ID);
    }

private:
    void setup_program(const std::string& vertex_path, const std::string& fragment_path) 
    {
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream v_shader_file;
        std::ifstream f_shader_file;
        // ensure ifstream objects can throw exceptions:
        v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            v_shader_file.open(vertex_path);
            f_shader_file.open(fragment_path);
            std::stringstream v_shader_stream, f_shader_stream;
            // read file's buffer contents into streams
            v_shader_stream << v_shader_file.rdbuf();
            f_shader_stream << f_shader_file.rdbuf();
            // close file handlers
            v_shader_file.close();
            f_shader_file.close();
            // convert stream into string
            vertex_code = v_shader_stream.str();
            fragment_code = f_shader_stream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* v_shader_code = vertex_code.c_str();
        const char* f_shader_code = fragment_code.c_str();

        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, NULL);
        glCompileShader(vertex);
        check_compile_errors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, NULL);
        glCompileShader(fragment);
        check_compile_errors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        check_compile_errors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void check_compile_errors(unsigned int shader, const std::string& type) 
    {
        int success;
        char info_log[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, info_log);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << "\n";
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, info_log);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log << "\n";
            }
        }
    }
};