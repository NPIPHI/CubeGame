//
// Created by 16182 on 10/27/2019.
//

#include "GameMap.h"
#include "iostream"


GameMap::GameMap(GLuint vBuffer, GLuint uvBuffer, GLuint iBuffer) {
    globalOffset = {0, 0, 0};
    vertexCount = chunck::chunckIndexCount * chunckLoadWidth * chunckLoadWidth * chunckLoadWidth;
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
                chunckArray[x][y][z] = chunck::load({globalOffset.x + x * CHUNKWIDTH, globalOffset.y + y * CHUNKWIDTH,
                                                     globalOffset.z + z * CHUNKWIDTH},
                                                    R"(C:\Users\16182\Documents\CPPStuff\MineCroft\mapData)");
                chunckArray[x][y][z]->setMainMap(this);
            }
        }
    }
    for(int x = 0; x < chunckLoadWidth; x ++){
        for(int y = 0; y < chunckLoadWidth; y ++) {
            for (int z = 0; z < chunckLoadWidth; z++) {
                chunckArray[x][y][z]->genMesh(vBuffer, uvBuffer, iBuffer,
                                              reinterpret_cast<void *>(chunck::chunckIndexCount *
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
                chunck::deLoad(chunckArray[x][y][z]);
            }
        }
    }
}

char GameMap::valueAt(int x, int y, int z) const{
    if(x < globalOffset.x || y < globalOffset.y || z < globalOffset.z ||
       x >= globalOffset.x + chunckLoadWidth * chunck::chunckWidth ||
       y >= globalOffset.y + chunckLoadWidth * chunck::chunckWidth ||
       z >= globalOffset.z + chunckLoadWidth * chunck::chunckWidth) {
        return 0;
    }
    intVect chunckOffset = (intVect(x, y, z) - globalOffset) / chunck::chunckWidth;
    intVect gridOffset = intVect(x, y, z) % chunck::chunckWidth;
    return chunckArray[chunckOffset.x][chunckOffset.y][chunckOffset.z]->valueAt(gridOffset);
}

void GameMap::changeBlock(int x, int y, int z, char blockID) {
    //slow, make faster if nessesary
    changeBlock(intVect(x, y, z), blockID);
}

chunck *GameMap::chunckAt(int x, int y, int z) const {
    return chunckAt(intVect(x, y ,z));
}

intVect GameMap::gridOffsetAt(int x, int y, int z) {
    return intVect(x, y, z) % chunck::chunckWidth;
}

chunck *GameMap::chunckAt(const intVect &pos) const {//don't truncate towrd 0
    intVect chunckOffset = ((pos - globalOffset + chunck::chunckWidth) / chunck::chunckWidth) - intVect(1);
    if (chunckOffset.x >= chunckLoadWidth || chunckOffset.x < 0 ||
        chunckOffset.y >= chunckLoadWidth || chunckOffset.y < 0 ||
        chunckOffset.z >= chunckLoadWidth || chunckOffset.z < 0) {
        return nullptr;
    }
    return chunckArray[chunckOffset.x][chunckOffset.y][chunckOffset.z];
}

char GameMap::valueAt(const intVect &pos) const{
    return valueAt(pos.x, pos.y, pos.z);
}

void GameMap::changeBlock(const intVect &pos, char blockID) {
    intVect gridOffset = gridOffsetAt(pos);
    chunck *effectedChunck = chunckAt(pos);
    if (effectedChunck != nullptr) {
        effectedChunck->setBlockData(gridOffset, blockID);
        effectedChunck->writeBlockMesh(gridOffset);
        for (const intVect &offset : adjancencyTable) {
            chunck *effectedChuck = chunckAt(pos + offset);
            if (effectedChuck != nullptr) {
                chunckAt(pos + offset)->writeBlockMesh(gridOffsetAt(pos + offset));
            }
        }
    }
}

intVect GameMap::gridOffsetAt(const intVect &pos) {
    return pos % chunck::chunckWidth;
}
