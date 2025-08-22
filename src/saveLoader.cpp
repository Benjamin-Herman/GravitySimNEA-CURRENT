#include "../headers/saveLoader.h"


struct OBJ {
    std::string name;
    std::string path;
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 acc;
    float mass;
};

struct CAM {
    glm::vec3 pos;
    float pitch;
    float yaw;
};

saveLoader::saveLoader(){}
std::vector<Object> saveLoader::loadSave(const std::string& savePath, Camera& cam) {
    readFile(savePath); // fills 'lines'

    std::vector<Object> SimObjs;
    std::vector<OBJ> objects;
    CAM camera{};

    bool inObjects = false;
    bool inCamera = false;
    OBJ currentObj;

    // Delimiters: split only on ',', '{', '}' — leave ':' and spaces intact for path parsing
    std::vector<char> delimiters = { ',', '{', '}', ':'};


    for (const std::string& line : lines) {
        std::vector<std::string> tokens = pystr::split(line, delimiters);

        for (std::size_t i = 0; i < tokens.size(); i++) {
            std::string tkn = tokens[i];
            //std::cout << tkn << "\n";
            if (tkn == "OBJECTS") {
                inObjects = true;
                inCamera = false;
                continue;
            }
            else if (tkn == "CAMERA") {
                inObjects = false;
                inCamera = true;
                continue;
            }
            else if (tkn == "UTIL") {
                inObjects = false;
                inCamera = false;
                continue;
            }


            else if (tkn == "}") {
                if (inObjects && !currentObj.name.empty()) {
                    objects.push_back(currentObj);
                    currentObj = {};
                }
                inCamera = false;
                continue;
            }

            if (inObjects) {
                if (currentObj.name.empty() && tkn != "LOCATION" && tkn != "POS" && tkn != "VEL" && tkn != "ACC" && tkn != "MASS" && tkn != "{" && tkn != "}" && tkn != ":") {
                    currentObj.name = tkn;
                }
                else if (tkn == "LOCATION") {
                    currentObj.path = tokens[i + 2];
                }
                else if (tkn == "POS") {
                    glm::vec3 p = glm::vec3{
                        safeStof(tokens[i + 2]),
                        safeStof(tokens[i + 4]),
                        safeStof(tokens[i + 6])
                    };
                    currentObj.pos = p;
                }
                else if (tkn == "VEL") {
                    glm::vec3 v = glm::vec3{
                        safeStof(tokens[i + 2]),
                        safeStof(tokens[i + 4]),
                        safeStof(tokens[i + 6])
                    };
                    currentObj.vel = v;
                }
                else if (tkn == "ACC") {
                    glm::vec3 a = glm::vec3{
                        safeStof(tokens[i + 2]),
                        safeStof(tokens[i + 4]),
                        safeStof(tokens[i + 6])
                    };
                    currentObj.acc = a;
                }
                else if (tkn == "MASS") {
                    currentObj.mass = stof(tokens[i + 2]);
                    std::cout << currentObj.mass << "\n";
                }
            }

            if (inCamera) {
                if (tkn == "POS") {
                    glm::vec3 p = glm::vec3{
                        safeStof(tokens[i + 2]),
                        safeStof(tokens[i + 4]),
                        safeStof(tokens[i + 6])
                    };
                    camera.pos = p;
                    //td::cout << p.x << p.y << p.z << "\n";
                }
                else if (tkn == "PITCH") {
                    float p = std::stof(tokens[i + 2]);
                    camera.pitch = p;
                }
                else if (tkn == "YAW") {
                    float y = std::stof(tokens[i + 2]);
                    camera.yaw = y;
                }
            }

        }


    }
    // Convert OBJ structs to Object structs
    for (OBJ obj : objects) {
        Object temp(obj.path);
        temp.setPosition(obj.pos);
        temp.setAcceleration(obj.acc);
        temp.setVelocity(obj.vel);
        temp.setMass(obj.mass);
        SimObjs.push_back(temp);
    }

    // Update camera
    //cam.Position = camera.pos;
    //cam.Pitch = camera.pitch;
    //cam.Yaw = camera.yaw;
    std::cout << SimObjs.size();
    return SimObjs;
}




void saveLoader::readFile(std::string path){
    std::ifstream file(path); //open file
    if (!file) { //check if open ok
        std::cerr << "Error opening file!" << std::endl;
    }

    std::string line;
    while (std::getline(file, line)) { //read each line
        lines.push_back(line);
    }

    file.close(); //close file
}
