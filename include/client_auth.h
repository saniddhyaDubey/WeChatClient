#pragma once
#include<iostream>
#include <format>
#include <string>
#include "config.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

bool loginUser(std::string username, std::string password, std::string secret_key);
bool registerUser(const std::string &username, const std::string &password, const std::string &secret_key);
