//
// Created by 16182 on 10/30/2019.
//

#include "character.h"
#include <iostream>

void character::update(const GameMap &worldMMap) {
    input.poll(window);
    if (input.getLeftToggle()) {
        std::cout << "left click" << std::endl;
    }
}
