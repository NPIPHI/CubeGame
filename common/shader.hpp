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
};

struct uniformContainer {
    union {
        glm::mat4 mat4;
    };
    union {
        glm::mat3 mat3;
    };
    union {
        glm::mat2 mat2;
    };
    union {
        glm::vec4 vec4;
    };
    union {
        glm::vec3 vec3;
    };
    union {
        glm::vec2 vec2;
    };

    uniformContainer(glm::mat4 mat4) : mat4(mat4) {}

    uniformContainer(glm::mat3 mat3) : mat3(mat3) {}

    uniformContainer(glm::mat2 mat2) : mat2(mat2) {}

    uniformContainer(glm::vec4 vec4) : vec4(vec4) {}

    uniformContainer(glm::vec3 vec3) : vec3(vec3) {}

    uniformContainer(glm::vec2 vec2) : vec2(vec2) {}

    uniformContainer() : mat4(0) {}
};

struct uniform {
    uniform(GLuint location, glmType type, uniformContainer value) : location(location), type(type), value(value) {};
    GLuint location;
    glmType type;
    uniformContainer value;
};

struct shader{
    GLuint shaderID;
    glm::mat4 matrix4;
    glm::vec3 vector3;
    std::map<std::string, uniform> uniforms;

    shader(const char *vertex_file_path, const char *fragment_file_path, const std::vector<std::string> &uniforms);
    void setUniformMatrix4(const char * name, const glm::mat4 &value);
    void setUniformVec3(const char * name, const glm::vec3 &value);
    void passUniforms() const;
};

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
