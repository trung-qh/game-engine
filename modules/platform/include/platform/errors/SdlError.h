#pragma once

#include <string>

namespace engine::platform {

std::string FormatSdlError(const char* operation);
[[noreturn]] void ThrowSdlError(const char* operation);
void CheckSdlResult(bool result, const char* operation);

}  // namespace engine::platform
