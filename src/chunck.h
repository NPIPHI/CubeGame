//
// Created by 16182 on 10/14/2019.
//

#ifndef MINECROFT_CHUNCK_H
#define MINECROFT_CHUNCK_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "IntVect.h"

class GameMap;

#define CHUNKWIDTH 8

//TODO corners generate mesh incorrectly on change block

class chunck {
public:
    static const size_t chunckSize = CHUNKWIDTH * CHUNKWIDTH * CHUNKWIDTH;
    static const size_t chunckWidth = CHUNKWIDTH;
    static const size_t chunckIndexCount = CHUNKWIDTH * CHUNKWIDTH * CHUNKWIDTH * 36;
    static const size_t chunckVertexCount = (CHUNKWIDTH + 1) * (CHUNKWIDTH + 1) * (CHUNKWIDTH + 1);
    intVect position{};

    const static void save(const chunck *chunck, const char *directory);

    static chunck *load(intVect position, const char *directroy);

    static chunck *generate(intVect position);

    static void deLoad(chunck *deleteChunck);
    void setMainMap(const GameMap* mainMap);
    void genMesh(const GLuint vertexBuffer, const GLuint UVBuffer, const GLuint indexBuffer, void *bufferOffset);
    void changeBlock(int x, int y, int z, char blockID);
    void setBlockData(int x, int y, int z, char blockID);
    void setBlockData(const intVect &pos, char blockID);
    void writeBlockMesh(const intVect &pos) const;

    chunck(char *data, intVect position, const GameMap *mainMap);
    char valueAt(intVect pos) const;

private:
    const GameMap* worldMap;
    bool modifyed = false;
    void * bufferOffset;
    char chunckData[CHUNKWIDTH * CHUNKWIDTH * CHUNKWIDTH]{};
    GLuint vertexBuffer, UVBuffer, indexBuffer;
    inline char valueAt(int x, int y, int z) const;
    inline char globalValueAt(int x, int y, int z) const;
    void genMeshAt(int x, int y, int z, glm::vec3 *vBuffer, glm::vec2 *uvBuffer, GLuint *iBuffer, int bufferOffset) const;
    inline void genMeshAt(intVect pos, glm::vec3 *vBuffer, GLuint *iBuffer, int bufferOffset) const;
    void placeFace(int x, int y, int z, glm::vec3 *vBuffer, glm::vec2 *uvBuffer, GLuint *iBuffer, int bufferOffset,
                   int faceID) const;
    inline int indexAt(int x, int y, int z) const;
    inline int indexAt(const intVect &pos) const;
    void writeBlockMesh(int x, int y, int z) const;
};

static const glm::vec3 cubeMesh[]{
        //-x
        glm::vec3{0,0,0},
        glm::vec3{0,0, 1.0f},
        glm::vec3{0, 1.0f, 1.0f},
        glm::vec3{0,0,0},
        glm::vec3{0, 1.0f, 1.0f},
        glm::vec3{0, 1.0f,0},
        //-y
        glm::vec3{1.0f,0, 1.0f},
        glm::vec3{0,0,0},
        glm::vec3{1.0f,0,0},
        glm::vec3{1.0f,0, 1.0f},
        glm::vec3{0,0, 1.0f},
        glm::vec3{0,0,0},
        //-z
        glm::vec3{1.0f, 1.0f,0},
        glm::vec3{0,0,0},
        glm::vec3{0, 1.0f,0},
        glm::vec3{1.0f, 1.0f,0},
        glm::vec3{ 1.0f,0,0},
        glm::vec3{0,0,0},
        //x
        glm::vec3{1.0f, 1.0f, 1.0f},
        glm::vec3{1.0f,0,0},
        glm::vec3{1.0f, 1.0f,0},
        glm::vec3{1.0f,0,0},
        glm::vec3{1.0f, 1.0f, 1.0f},
        glm::vec3{1.0f,0, 1.0f},
        //y
        glm::vec3{1.0f, 1.0f, 1.0f},
        glm::vec3{1.0f, 1.0f,0},
        glm::vec3{0, 1.0f,0},
        glm::vec3{1.0f, 1.0f, 1.0f},
        glm::vec3{0, 1.0f,0},
        glm::vec3{0, 1.0f, 1.0f},
        //z
        glm::vec3{0, 1.0f, 1.0f},
        glm::vec3{0,0, 1.0f},
        glm::vec3{1.0f,0, 1.0f},
        glm::vec3{1.0f, 1.0f, 1.0f},
        glm::vec3{0, 1.0f, 1.0f},
        glm::vec3{1.0f,0, 1.0f}
};

#endif //MINECROFT_CHUNCK_H