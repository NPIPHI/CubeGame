//
// Created by 16182 on 10/27/2019.
//

#ifndef MINECROFT_GAMEMAP_H
#define MINECROFT_GAMEMAP_H

#include "Chunck.h"
#include "IntVect.h"
#include <array>

class GameMap {
public:
    GameMap(GLuint vBuffer, GLuint uvBuffer, GLuint iBuffer);
    ~GameMap();
    int getVertexCount() const;
    char valueAt(int x, int y, int z) const;
    char valueAt(const intVect &pos) const;
    void changeBlock(int x, int y, int z, char blockID);
    void changeBlock(const intVect &pos, char blockID);
    Chunck* chunckAt(int x, int y, int z) const;
    Chunck* chunckAt(const intVect &pos) const;

private:
    GLuint vBuffer, uvBuffer, iBuffer;
    static const int chunckLoadWidth = 2;
    int vertexCount;
    intVect globalOffset;
    Chunck *chunckArray[chunckLoadWidth][chunckLoadWidth][chunckLoadWidth];

    inline intVect gridOffsetAt(int x, int y, int z);
    inline intVect gridOffsetAt(const intVect &pos);
};

static const std::array<intVect, 6> adjancencyTable = {
        intVect{-1, 0, 0},
        intVect{0, -1, 0},
        intVect{0, 0, -1},
        intVect{1, 0, 0},
        intVect{0, 1, 0},
        intVect{0, 0, 1}
};

#endif //MINECROFT_GAMEMAP_H
