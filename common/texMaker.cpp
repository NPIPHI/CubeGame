//
// Created by 16182 on 8/28/2019.
//

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include "common/texture.hpp"
#include "texMaker.h"

GLuint texMaker::loadTextureRGB(const char *path) {
    //return loadBMP_custom(path);
}

GLuint texMaker::writeTextureBW(GLuint textureID, const unsigned char *data, const int width, const int height,
                                GLenum filter=GL_LINEAR) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    return textureID;
}

GLuint texMaker::writeTextureRGB(GLuint textureID,const unsigned char *data,const int width,const int height) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    return textureID;
}

GLuint texMaker::writeTextureRGBA(GLuint textureID, const unsigned char *data, const int width, const int height) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    return textureID;
}

GLuint texMaker::genTextureArray(const int width, const int height, const int layers) {
    const GLsizei mipLevelCount = 1;
    GLuint texArrayID;
    glGenBuffers(1, &texArrayID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texArrayID);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGB, width, height, layers);
    return texArrayID;
}

GLuint texMaker::writeTextureArray(GLuint arrayID, const unsigned char *data, const int width, const int height,
                                   const int layers) {
    const GLsizei mipLevelCount = 1;
    const GLsizei layerCount = layers;
    glBindTexture(GL_TEXTURE_2D_ARRAY, arrayID);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGB, width, height, layerCount);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, GL_RGB, GL_UNSIGNED_BYTE, data);
    return arrayID;
}

GLuint texMaker::writeSubTextureArray(GLuint arrayID, const unsigned char *data, const int width, const int height,
                                      const int layer) {
    glBindTexture(GL_TEXTURE_2D_ARRAY, arrayID);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
    return arrayID;
}

GLuint texMaker::loadArrayTextureRGB(const std::vector<const char *> &paths) {
    GLuint texArrayID;
    {
        unsigned int width, height, depth;
        FILE *metaFile = fopen(paths[0], "rb");
        unsigned char metaHeader[54];
        fread(metaHeader, 1, 54, metaFile);
        width = *(int *) &(metaHeader[0x12]);
        height = *(int *) &(metaHeader[0x16]);
        depth = paths.size();
        fclose(metaFile);
        texArrayID = genTextureArray(width, height, depth);
    }

    for(int pathID = 0; pathID < paths.size(); pathID++){
        const char * imagepath = paths[pathID];
        printf("Reading image %s\n", imagepath);

        // Data read from the header of the BMP file
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int imageSize;
        unsigned int width, height;
        // Actual RGB data
        unsigned char * data;

        // Open the file
        FILE * file = fopen(imagepath,"rb");
        if (!file){
            printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
            getchar();
            return 0;
        }

        // Read the header, i.e. the 54 first bytes

        // If less than 54 bytes are read, problem
        if ( fread(header, 1, 54, file)!=54 ){
            printf("Not a correct BMP file\n");
            fclose(file);
            return 0;
        }
        // A BMP files always begins with "BM"
        if ( header[0]!='B' || header[1]!='M' ){
            printf("Not a correct BMP file\n");
            fclose(file);
            return 0;
        }
        // Make sure this is a 24bpp file
        if ( *(int*)&(header[0x1E])!=0  ){
            printf("Not a correct BMP file\n");
            fclose(file);
            return 0;
        }

        if ( *(int*)&(header[0x1C])!=24 ){
            printf("Not a correct BMP file\n");
            fclose(file);
            return 0;
        }

        // Read the information about the image
        dataPos    = *(int*)&(header[0x0A]);
        imageSize  = *(int*)&(header[0x22]);
        width      = *(int*)&(header[0x12]);
        height     = *(int*)&(header[0x16]);

        // Some BMP files are misformatted, guess missing information
        if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
        if (dataPos==0)      dataPos=54; // The BMP header is done that way

        // Create a buffer
        data = new unsigned char [imageSize + dataPos];

        // Read the actual data from the file into the buffer
        fread(data,1,imageSize + dataPos,file);

        // Everything is in memory now, the file can be closed.
        fclose (file);

        writeSubTextureArray(texArrayID, data + dataPos, width, height, pathID);
        for(int i = 0; i < 16; i ++){
            for(int j = 0; j < 16; j ++){
                std::cout << (data+dataPos)[3*(16*i+j)];
                //std::cout << (data+dataPos)[3*(16*i+j)+1];
                //std::cout << (data+dataPos)[3*(16*i+j)+2];
            }
            std::cout << std::endl;
        }
        delete [] data;
    }
    //glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    std::cout << "\ndone reading in\n" << std::endl;
    char readBuffer[256 * 3];
    glGetBufferSubData(GL_TEXTURE_2D_ARRAY, 0, 256*3, readBuffer);
    for(int i = 0; i < 16; i ++){
        for(int j = 0; j < 16; j ++){
            std::cout << readBuffer[3*(i*16 + j)];
        }
        std::cout << std::endl;
    }
    return texArrayID;
}
