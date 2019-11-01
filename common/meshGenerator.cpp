//
// Created by 16182 on 10/3/2019.
//

#include "meshGenerator.h"


unsigned char valueAt(const unsigned char * scalarField, int sideLength, int x, int y, int z){
    if(x < 0 || y < 0 || z < 0 || x >= sideLength || y >= sideLength || z >= sideLength) return 0;
    return scalarField[x + y * sideLength + z * sideLength * sideLength];
}

std::vector<unsigned char> genScalarField(unsigned int sideLength, int repeats, int seed) {
    PerlinNoise pNoise(seed);
    std::vector<unsigned char> scalarField;
    scalarField.reserve(sideLength*sideLength*sideLength);

    for(unsigned int x = 0; x < sideLength; x++){
        for(unsigned int y = 0; y < sideLength; y++){
            for(unsigned int z = 0; z < sideLength; z++){
                scalarField.emplace_back(pNoise.noise(double(x)/sideLength*repeats, double(y)/sideLength*repeats, double(z)/sideLength*repeats) * 255);
            }
        }
    }

    return scalarField;
}

std::vector<glm::vec3> genMesh(const unsigned char *scalarField, int sideLength) {
    const unsigned char threashold = 128;
    const unsigned int cellLength = sideLength - 1; //cube root of the number of cubes in the mesh;
    const unsigned int cellNumber = cellLength * cellLength * cellLength; //number of cells represented by the scalar field
    std::vector<glm::vec3> vertexBuffer;
    //count vertices
    unsigned int vertexCount = 0;

    for(int x = 0; x < cellLength; x++){
        for(int y = 0; y < cellLength; y++){
            for(int z = 0; z < cellLength; z++){
                unsigned char cubeIndex = 0;
                if (valueAt(scalarField, sideLength, x, y, z) > threashold) cubeIndex += 1;
                if (valueAt(scalarField, sideLength, x + 1, y, z) > threashold) cubeIndex += 2;
                if (valueAt(scalarField, sideLength, x + 1, y, z + 1) > threashold) cubeIndex += 4;
                if (valueAt(scalarField, sideLength, x, y, z + 1) > threashold) cubeIndex += 8;
                if (valueAt(scalarField, sideLength, x, y+1, z) > threashold) cubeIndex += 16;
                if (valueAt(scalarField, sideLength, x+1, y+1, z) > threashold) cubeIndex += 32;
                if (valueAt(scalarField, sideLength, x+1, y+1, z+1) > threashold) cubeIndex += 64;
                if (valueAt(scalarField, sideLength, x, y+1, z+1) > threashold) cubeIndex += 128;
                const char * edges = triTable[cubeIndex];
                int triNumber = 0;
                for(int triIndex = 0; triIndex<16; triIndex+=3){
                    if(edges[triIndex]!=-1){
                        triNumber++;
                    }
                }
                for(int triIndex = 0; triIndex<triNumber*3; triIndex++){
                    glm::vec3 vertex = (edgeTable[edges[triIndex]][0] + edgeTable[edges[triIndex]][1]);
                    vertex /= 2;
                    vertex += glm::vec3(x, y, z);
                    vertexBuffer.push_back(vertex);
                }
            }
        }
    }

    return vertexBuffer;
}

std::vector<glm::vec3> genCubeVertices(const unsigned char *scalarField, int sideLength) {
    std::vector<glm::vec3> vertexBuffer;
    //count cubes
    unsigned int cubeCount = 0;
    for(int i = 0; i < sideLength * sideLength * sideLength; i++){
        if(scalarField[i] > 128){
            cubeCount ++;
        }
    }

    vertexBuffer.reserve(cubeCount);
    const int xJump = 1;
    const int yJump = sideLength;
    const int zJump = sideLength * sideLength;
    for(int x = 0; x < sideLength; x++) {
        for (int y = 0; y < sideLength; y++) {
            for (int z = 0; z < sideLength; z++) {
                if (scalarField[x * xJump + y * yJump + z * zJump]>128) {
                    vertexBuffer.emplace_back(x, y, z);
                }
            }
        }
    }
    return vertexBuffer;
}

std::vector<glm::vec3> genCubeVerticesBorder(const unsigned char *scalarField, int sideLength) {
    std::vector<glm::vec3> vertexBuffer;
    //count cubes
    const unsigned char threashold = 128;
    unsigned int cubeCount = 0;
    for (int x = 0; x < sideLength; ++x) {
        for (int y = 0; y < sideLength; ++y) {
            for (int z = 0; z < sideLength; ++z) {
                if(valueAt(scalarField, sideLength, x, y, z) > threashold) {
                    if (x==0 || y==0 || z==0 || x==sideLength-1 || y==sideLength-1 || z==sideLength-1) {
                        cubeCount++;
                    } else if (!(valueAt(scalarField, sideLength, x - 1, y, z) > threashold &&
                          valueAt(scalarField, sideLength, x + 1, y, z) > threashold &&
                          valueAt(scalarField, sideLength, x, y - 1, z) > threashold &&
                          valueAt(scalarField, sideLength, x, y + 1, z) > threashold &&
                          valueAt(scalarField, sideLength, x, y, z - 1) > threashold &&
                          valueAt(scalarField, sideLength, x, y, z + 1) > threashold)) {
                        cubeCount++;
                    }
                }
            }
        }
    }

    vertexBuffer.reserve(cubeCount);
    for (int x = 0; x < sideLength; ++x) {
        for (int y = 0; y < sideLength; ++y) {
            for (int z = 0; z < sideLength; ++z) {
                if(valueAt(scalarField, sideLength, x, y, z) > threashold){
                    if (x==0 || y==0 || z==0 || x==sideLength-1 || y==sideLength-1 || z==sideLength-1) {
                        vertexBuffer.emplace_back(x, y, z);
                    } else if (!(valueAt(scalarField, sideLength, x - 1, y, z) > threashold &&
                                 valueAt(scalarField, sideLength, x + 1, y, z) > threashold &&
                                 valueAt(scalarField, sideLength, x, y - 1, z) > threashold &&
                                 valueAt(scalarField, sideLength, x, y + 1, z) > threashold &&
                                 valueAt(scalarField, sideLength, x, y, z - 1) > threashold &&
                                 valueAt(scalarField, sideLength, x, y, z + 1) > threashold)) {
                        vertexBuffer.emplace_back(x, y, z);
                    }
                }
            }
        }
    }
    return vertexBuffer;
}
