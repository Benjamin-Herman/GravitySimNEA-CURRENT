#pragma once
#ifndef SQLManager_H
#define SQLManager_H

#include <iostream>

class SQL {
public:
	int init();
	void loop();
	bool cont;
	void login(const std::string& user, const std::string& pass);
	void signup(const std::string& user, const std::string& pass);
	std::string hashText(std::string toHash);
private:

	

};

#endif