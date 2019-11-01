//
// Created by 16182 on 8/28/2019.
//

#ifndef TUTORIALS_TEXMAKER_H
#define TUTORIALS_TEXMAKER_H


#include <GL/gl.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

class texMaker {
private:
public:
    static GLuint loadTextureRGB(const char * path);
    static GLuint loadArrayTextureRGB(const std::vector<const char *> & paths);
    static GLuint genTextureArray(const int width, const int height, const int layers);
    static GLuint writeTextureArray(GLuint arrayID, const unsigned char *data, const int width, const int height,
                                    const int layers);
    static GLuint writeSubTextureArray(GLuint arrayID, const unsigned char *data, const int width, const int height, const int layer);
    static GLuint
    writeTextureBW(GLuint textureID, const unsigned char *data, const int width, const int height, GLenum filter);
    static GLuint writeTextureRGB(GLuint textureID, const unsigned char *data, const int width, const int height);
    static GLuint writeTextureRGBA(GLuint textureID, const unsigned char *data, const int width, const int height);
};


#endif //TUTORIALS_TEXMAKER_H
