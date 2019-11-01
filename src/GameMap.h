//
// Created by 16182 on 10/27/2019.
//

#ifndef MINECROFT_GAMEMAP_H
#define MINECROFT_GAMEMAP_H

#include "Chunck.h"
#include "IntVect.h"

class GameMap {
public:
    GameMap(GLuint vBuffer, GLuint uvBuffer, GLuint iBuffer);
    ~GameMap();
    int getVertexCount() const;
    char valueAt(int x, int y, int z) const;
    void setBlock(int x, int y, int z, char blockID);

private:
    GLuint vBuffer, uvBuffer, iBuffer;
    static const int chunckLoadWidth = 8;
    int vertexCount;
    intVect globalOffset;
    Chunck *chunckArray[chunckLoadWidth][chunckLoadWidth][chunckLoadWidth];
};


#endif //MINECROFT_GAMEMAP_H
