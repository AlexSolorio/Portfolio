//
//  surfaceExtraction.hpp
//  Homework7
//
//  Created by Ben Jones on 1/6/17.
//  Copyright © 2017 Ben Jones. All rights reserved.
//

#pragma once
#include <cstdint>
#include "VoxelShape.hpp"

void WriteSTL(const VoxelShape &model, const char* filename, int numThreads);
