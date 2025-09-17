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
    readFile(savePath); //reads file and fills the lines vector with data, no returns needed 

    std::vector<Object> SimObjs; //the list used in the sim itself
    std::vector<OBJ> objects; //the list of the local struct so I can create the sim obj later
    CAM camera{}; //camera struct

    bool inObjects = false; //inside which part of the file
    bool inCamera = false;
    OBJ currentObj;

    //a list of all the characters that i want to split by
    std::vector<char> delimiters = { ',', '{', '}', ':'};


    for (const std::string& line : lines) { //go line by line
        std::vector<std::string> tokens = pystr::split(line, delimiters); //split line into vector of usefull tokens

        for (std::size_t i = 0; i < tokens.size(); i++) {
            std::string tkn = tokens[i];
            //std::cout << tkn << "\n";
            if (tkn == "OBJECTS") { //major checks for location in file
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


            else if (tkn == "}") { //when the object is finished reading, add to the list
                if (inObjects && !currentObj.name.empty()) {
                    objects.push_back(currentObj);
                    currentObj = {};
                }
                inCamera = false;
                continue;
            }

            if (inObjects) {
                if (currentObj.name.empty() && tkn != "LOCATION" && tkn != "POS" && tkn != "VEL" && tkn != "ACC" && tkn != "MASS" && tkn != "{" && tkn != "}" && tkn != ":") {
                    currentObj.name = tkn; //checked if its the name currently being read
                }
                else if (tkn == "LOCATION") {
                    currentObj.path = tokens[i + 2]; 
                }
                else if (tkn == "POS") { //creates the vec3 for all these datasets and float for mass
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
                    //std::cout << currentObj.mass << "\n";
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
    //convert struct data into sim data
    for (OBJ obj : objects) {
        std::cout << "Loaded objects " << obj.name << "\n";
        Object temp(obj.path);
        temp.setPosition(obj.pos);
        temp.setAcceleration(obj.acc);
        temp.setVelocity(obj.vel);
        temp.setMass(obj.mass);
        SimObjs.push_back(temp);
    }

    //update camera start values
    cam.Position = camera.pos;
    cam.Pitch = camera.pitch;
    cam.Yaw = camera.yaw;
    //std::cout << SimObjs.size();
    cam.updateCameraVectors();
    lines.clear();
    return SimObjs;
}




void saveLoader::readFile(std::string path){
    std::ifstream file(path); //open file
    if (!file) { //check if open ok
        std::cerr << "Error opening save file!" << std::endl;
    }

    std::string line;
    while (std::getline(file, line)) { //read each line
        lines.push_back(line);
    }

    file.close(); //close file
}
