//This file will probably be deleted

#pragma once

#ifndef MODELS_HPP
#define MODELS_HPP

#include "util.hpp"

namespace Models
{

struct Map
{
    Map() = default;
    Map(const TILEARRAY& tex_, const TILEARRAY& obs_) : tex{tex_}, obs{obs_} {}
    ~Map() = default;

    TILEARRAY tex{};
    TILEARRAY obs{};
};

}

#endif // MODELS_HPP
