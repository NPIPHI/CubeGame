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

void GameMap::setBlock(int x, int y, int z, char blockID) {
    intVect chunckOffset = (intVect(x, y, z) - globalOffset) / Chunck::chunckWidth;
    intVect gridOffset = intVect(x, y, z) % Chunck::chunckWidth;
    chunckArray[chunckOffset.x][chunckOffset.y][chunckOffset.z]->changeBlock(gridOffset.x, gridOffset.y, gridOffset.z, blockID);
}
