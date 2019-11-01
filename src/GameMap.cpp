//
// Created by 16182 on 10/27/2019.
//

#include "GameMap.h"

GameMap::GameMap(GLuint vBuffer, GLuint uvBuffer, GLuint iBuffer) {
    globalOffset = {0, 0, 0};
    vertexCount = Chunck::chunckIndexCount * chunckLoadWidth * chunckLoadWidth * chunckLoadWidth;
    this->vBuffer = vBuffer;
    this->uvBuffer = uvBuffer;
    this->iBuffer = iBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec2), nullptr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount * sizeof(GLuint), nullptr, GL_STATIC_DRAW);

    for(int x = 0; x < chunckLoadWidth; x ++){
        for(int y = 0; y < chunckLoadWidth; y ++) {
            for (int z = 0; z < chunckLoadWidth; z++) {
                chunckArray[x][y][z] = Chunck::load({globalOffset.x + x*CHUNKWIDTH, globalOffset.y + y*CHUNKWIDTH, globalOffset.z + z*CHUNKWIDTH}, R"(C:\Users\16182\Documents\CPPStuff\MineCroft\mapData)");
                chunckArray[x][y][z]->setMainMap(this);
            }
        }
    }
    for(int x = 0; x < chunckLoadWidth; x ++){
        for(int y = 0; y < chunckLoadWidth; y ++) {
            for (int z = 0; z < chunckLoadWidth; z++) {
                chunckArray[x][y][z]->genMesh(vBuffer, uvBuffer, iBuffer,
                                              reinterpret_cast<void *>(Chunck::chunckIndexCount *
                                                                       (x + chunckLoadWidth * y +
                                                                        chunckLoadWidth * chunckLoadWidth * z)));
            }
        }
    }
}

int GameMap::getVertexCount() const{
    return vertexCount;
}

GameMap::~GameMap() {
    for(int x = 0; x < chunckLoadWidth; x ++){
        for(int y = 0; y < chunckLoadWidth; y ++) {
            for (int z = 0; z < chunckLoadWidth; z++) {
                Chunck::deLoad(chunckArray[x][y][z]);
            }
        }
    }
}

char GameMap::valueAt(int x, int y, int z) const{
    if(x < globalOffset.x || y < globalOffset.y || z < globalOffset.z ||
        x >= globalOffset.x + chunckLoadWidth * Chunck::chunckWidth ||
        y >= globalOffset.y + chunckLoadWidth * Chunck::chunckWidth ||
        z >= globalOffset.z + chunckLoadWidth * Chunck::chunckWidth){
        return 0;
    }
    intVect chunckOffset = (intVect(x, y, z) - globalOffset) / Chunck::chunckWidth;
    intVect gridOffset = intVect(x, y, z) % Chunck::chunckWidth;
    chunckArray[chunckOffset.x][chunckOffset.y][chunckOffset.z]->valueAt(gridOffset);
}

void GameMap::changeBlock(int x, int y, int z, char blockID) {
    //slow, make faster if nessesary
    changeBlock(intVect(x, y, z), blockID);
}

Chunck* GameMap::chunckAt(int x, int y, int z) const{
    return chunckAt(intVect(x, y ,z));
}

intVect GameMap::gridOffsetAt(int x, int y, int z) {
    return intVect(x, y, z) % Chunck::chunckWidth;
}

Chunck *GameMap::chunckAt(const intVect &pos) const {
    intVect chunckOffset = (pos - globalOffset) / Chunck::chunckWidth;
    return chunckArray[chunckOffset.x][chunckOffset.y][chunckOffset.z];
}

char GameMap::valueAt(const intVect &pos) const{
    return valueAt(pos.x, pos.y, pos.z);
}

void GameMap::changeBlock(const intVect &pos, char blockID) {
    intVect gridOffset = gridOffsetAt(pos);
    chunckAt(pos)->setBlockData(gridOffset, blockID);
    chunckAt(pos)->writeBlockMesh(gridOffset);
    for (const intVect &offset : adjancencyTable) {
        chunckAt(pos + offset)->writeBlockMesh(gridOffsetAt(pos + offset));
    }
}

intVect GameMap::gridOffsetAt(const intVect &pos) {
    return pos % Chunck::chunckWidth;
}
