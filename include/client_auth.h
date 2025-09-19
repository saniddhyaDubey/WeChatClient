#pragma once
#include<iostream>
#include <format>
#include <string>
#include "config.h"
#include <cpr/cpr.h>

bool loginUser(std::string username, std::string password);
bool registerUser(const std::string &username, const std::string &password, const std::string &secret_key);
