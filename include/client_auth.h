#pragma once
#include<iostream>

bool LoginUser(std::string username, std::string password, std::string secret_key);
bool registerUser(const std::string &username, const std::string &password, const std::string &secret_key);