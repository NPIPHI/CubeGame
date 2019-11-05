//
// Created by 16182 on 10/14/2019.
//

#include "chunck.h"

#include <cstdio>
#include <string>
#include <algorithm>
#include "GameMap.h"

const void chunck::save(const chunck *chunck, const char *directory) {
    std::FILE* currentFile;
    std::string fileName = std::to_string(chunck->position.x)+','+std::to_string(chunck->position.y)+','+std::to_string(chunck->position.z);
    fileName =  std::string(directory) + '/' + fileName + ".txt";
    currentFile = std::fopen(fileName.c_str(), "wb");
    std::fwrite(chunck->chunckData, sizeof(char), chunck::chunckSize, currentFile);
    std::fclose(currentFile);
}

chunck::chunck(char *data, intVect position, const GameMap *mainMap) {
    std::copy(data, data + chunck::chunckSize, chunckData);
    this->position = position;
    this->worldMap = mainMap;
    this->bufferOffset = 0;
    this->indexBuffer = 0;
    this->vertexBuffer = 0;
    this->UVBuffer = 0;
}

chunck *chunck::load(intVect position, const char *directroy) {
    std::FILE* loadFile;
    std::string fileName = std::string(directroy) + std::string(R"(\)") + std::to_string(position.x) + ',' + std::to_string(position.y) + ',' + std::to_string(position.z) + ".txt";
    if(loadFile = fopen(fileName.c_str(), "rb")){
        char buffer[chunck::chunckSize];
        fread(buffer, chunck::chunckSize, sizeof(char), loadFile);
        fclose(loadFile);
        return new chunck(buffer, position, nullptr);
    } else {
        return chunck::generate(position);
    }
}

chunck *chunck::generate(intVect position) {
    char buffer[chunck::chunckSize]{};
    for (int z = 0; z < chunck::chunckWidth; z++) {
        for (int y = 0; y < chunck::chunckWidth; y++) {
            for (int x = 0; x < chunck::chunckWidth; x++) {
                if(y + position.y < 32) {
                    buffer[x + y * chunck::chunckWidth + z * chunck::chunckWidth * chunck::chunckWidth] = 1;
                }
            }
        }
    }
    return new chunck(buffer, position, nullptr);
}

void chunck::deLoad(chunck *deleteChunck) {
    if(deleteChunck->modifyed){
        chunck::save(deleteChunck, R"(C:\Users\16182\Documents\CPPStuff\MineCroft\mapData)");
    }
    delete deleteChunck;
}

void chunck::genMesh(const GLuint vertexBuffer, const GLuint UVBuffer, const GLuint indexBuffer, void *bufferOffset) {
    this->bufferOffset = bufferOffset;
    this->vertexBuffer = vertexBuffer;
    this->indexBuffer = indexBuffer;
    this->UVBuffer = UVBuffer;
    glm::vec3 *vBuffer = new glm::vec3[chunck::chunckIndexCount];
    glm::vec2 *uvBuffer = new glm::vec2[chunck::chunckIndexCount];
    GLuint *iBuffer = new GLuint[chunck::chunckIndexCount]{};//index buffer must be initilized to 0, very important
    for (int z = 0; z < chunck::chunckWidth; z++) {
        for (int y = 0; y < chunck::chunckWidth; y++) {
            for (int x = 0; x < chunck::chunckWidth; x++) {
                genMeshAt(x, y, z, vBuffer, uvBuffer, iBuffer, indexAt(x, y, z) * 36);
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, int(bufferOffset) * sizeof(glm::vec3),
                    chunck::chunckIndexCount * sizeof(glm::vec3), vBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, int(bufferOffset) * sizeof(glm::vec2),
                    chunck::chunckIndexCount * sizeof(glm::vec2), uvBuffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, int(bufferOffset) * sizeof(GLuint),
                    chunck::chunckIndexCount * sizeof(GLuint), iBuffer);

    delete[] vBuffer;
    delete[] uvBuffer;
    delete[] iBuffer;
}

void chunck::genMeshAt(int x, int y, int z, glm::vec3 *vBuffer, glm::vec2 *uvBuffer, GLuint *iBuffer,
                       int bufferOffset) const {//buffer offset in vec3
    char blockID = valueAt(x, y, z);
    if(blockID){//For Solid Blocks
        if(!valueAt(x - 1, y, z)){
            placeFace(x, y, z, vBuffer, uvBuffer, iBuffer, bufferOffset, 0);
        }
        if(!valueAt(x, y-1, z)){
            placeFace(x, y, z, vBuffer, uvBuffer, iBuffer, bufferOffset, 1);
        }
        if(!valueAt(x, y, z-1)){
            placeFace(x, y, z, vBuffer, uvBuffer, iBuffer, bufferOffset, 2);
        }
        if(!valueAt(x+1, y, z)){
            placeFace(x, y, z, vBuffer, uvBuffer, iBuffer, bufferOffset, 3);
        }
        if(!valueAt(x, y+1, z)){
            placeFace(x, y, z, vBuffer, uvBuffer, iBuffer, bufferOffset, 4);
        }
        if(!valueAt(x, y, z+1)){
            placeFace(x, y, z, vBuffer, uvBuffer, iBuffer, bufferOffset, 5);
        }
    }
}

int chunck::indexAt(int x, int y, int z) const {
    return (x + y * chunck::chunckWidth + z * chunck::chunckWidth * chunck::chunckWidth);
}

int chunck::indexAt(const intVect &pos) const {
    return indexAt(pos.x, pos.y, pos.z);
}

char chunck::valueAt(int x, int y, int z) const {
    if (x < 0 || y < 0 || z < 0 || x >= chunck::chunckWidth || y >= chunck::chunckWidth || z >= chunck::chunckWidth) {
        return globalValueAt(x + position.x, y + position.y, z + position.z);
    }
    return chunckData[indexAt(x, y, z)];
}

char chunck::valueAt(intVect pos) const {
    return valueAt(pos.x, pos.y, pos.z);
}

void chunck::placeFace(int x, int y, int z, glm::vec3 *vBuffer, glm::vec2 *uvBuffer, GLuint *iBuffer, int bufferOffset,
                       int faceID) const{
    glm::vec3 offset = glm::vec3(position.x + x, position.y + y, position.z + z);
    for(int i = faceID * 6; i < (faceID + 1) * 6; i ++){
        vBuffer[bufferOffset+i] = cubeMesh[i] + offset;
        uvBuffer[bufferOffset+i] = glm::vec2(float((faceID/2)%3)/2, faceID%2);
        iBuffer[bufferOffset+i] = int(this->bufferOffset) + indexAt(x, y, z) * 36 + i;
    }
}

void chunck::genMeshAt(intVect pos, glm::vec3 *vBuffer, GLuint *iBuffer, int bufferOffset) const {
    genMeshAt(pos.x, pos.y, pos.z, vBuffer, nullptr, iBuffer, bufferOffset);
}

void chunck::changeBlock(int x, int y, int z, char blockID) {//move whole function to gmaeMap
    modifyed = true;
    chunckData[indexAt(x, y, z)] = blockID;
    if(x>0){//make it world write block mesh
        writeBlockMesh(x-1, y, z);
    }
    if(y>0){
        writeBlockMesh(x, y-1, z);
    }
    if(z>0){
        writeBlockMesh(x, y, z-1);
    }
    if (x < chunck::chunckWidth - 1) {
        writeBlockMesh(x+1, y, z);
    }
    if (y < chunck::chunckWidth - 1) {
        writeBlockMesh(x, y+1, z);
    }
    if (z < chunck::chunckWidth - 1) {
        writeBlockMesh(x, y, z+1);
    }
    writeBlockMesh(x, y, z);
}

void chunck::writeBlockMesh(int x, int y, int z) const {
    glm::vec3 vBuffer[36]{};
    glm::vec2 uvBuffer[36]{};
    GLuint iBuffer[36]{}; //fill with 0
    genMeshAt(x, y, z, vBuffer, uvBuffer, iBuffer, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, int(bufferOffset) * sizeof(glm::vec3) + indexAt(x, y, z) * 36 * sizeof(glm::vec3), 36 * sizeof(glm::vec3), vBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, int(bufferOffset) * sizeof(glm::vec2) + indexAt(x, y, z) * 36 * sizeof(glm::vec2), 36 * sizeof(glm::vec2), uvBuffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, int(bufferOffset) * sizeof(GLuint) + indexAt(x, y, z) * 36 * sizeof(GLuint), 36 * sizeof(GLuint), iBuffer);
}

char chunck::globalValueAt(int x, int y, int z) const {
    if(worldMap!= nullptr){
        return worldMap->valueAt(x, y, z);
    }
    return 0;
}

void chunck::setMainMap(const GameMap *mainMap) {
    worldMap = mainMap;
}

void chunck::setBlockData(int x, int y, int z, char blockID) {
    chunckData[indexAt(x, y, z)] = blockID;
}

void chunck::setBlockData(const intVect &pos, char blockID) {
    modifyed = true;
    chunckData[indexAt(pos)] = blockID;
}

void chunck::writeBlockMesh(const intVect &pos) const {
    writeBlockMesh(pos.x, pos.y, pos.z);
}
