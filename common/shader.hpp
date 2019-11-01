#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

enum glmType{
    none,
    mat4,
    mat3,
    mat2,
    vec4,
    vec3,
    vec2,
    vec1
};

struct shader{
    GLuint shaderID;
    glm::mat4 matrix4;
    glm::vec3 vector3;
    std::map<std::string, std::pair<GLuint, glmType>> uniforms;
    shader(const char * vertex_file_path, const char * fragment_file_path, std::vector<std::string> uniforms);
    void setUniformMatrix4(const char * name, const glm::mat4 &value);
    void setUniformVec3(const char * name, const glm::vec3 &value);
    void passUniforms() const;
};

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
