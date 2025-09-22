#pragma once
#ifndef SQLManager_H
#define SQLManager_H

#include <iostream>

class SQL {
public:
	int init();
	void loop();
	bool cont;
private:

	std::string hashText(std::string toHash);

};

#endif