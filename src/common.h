#pragma once

#include <cstdint>
#include <memory>

#define SQUARE(X) ((X)*(X))

#define PIMPL                                                                 \
	private:                                                              \
	struct Impl;                                                          \
	std::unique_ptr<Impl> d_;

namespace nealrame 
{
using real = float;
}
