#pragma once
#include <vector>
#include "object.h"
#include "simData.h"

//BIG NOTE ON PASS BY REFERENCE AND VALUE. TOOK A FEW HOURS TO FIND THAT I NEEDED TO SET ACCELERATION ETC BY REFERENECE NOT VALUE
void gravitySystemUpdate(std::vector<Object>& objs) {
    //reset all accelerations
    for (auto& obj : objs) {
        obj.setAcceleration(glm::vec3(0.0f));
    }

    for (size_t i = 0; i < objs.size(); i++) {
        for (size_t j = i + 1; j < objs.size(); ++j) {
            glm::vec3 r = objs[j].getPosition() - objs[i].getPosition();
            float dist = glm::length(r);
            float distSq = dist * dist + 1e-6f; //tiny value to avoid div by zero
            float force = gravityConstant / (distSq * dist);  // = G / dist³

            glm::vec3 acc = r * force;

            glm::vec3 accI = objs[i].getAcceleration() + acc * objs[j].getMass();
            glm::vec3 accJ = objs[j].getAcceleration() - acc * objs[i].getMass();

            objs[i].setAcceleration(accI);
            objs[j].setAcceleration(accJ);
        }
    }
}