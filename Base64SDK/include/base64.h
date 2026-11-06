
#pragma once

#include <cstddef>
#include <cstdint>

// Експорт функцій для DLL
#ifdef BASE64_EXPORTS
#define BASE64_API __declspec(dllexport)
#else
#define BASE64_API __declspec(dllimport)
#endif

extern "C" {

    BASE64_API std::size_t encodedSize(std::size_t inputSize);

    BASE64_API bool encode(
        const std::uint8_t* input,
        std::size_t inputSize,
        char* output,
        std::size_t outputCapacity,
        std::size_t* outputSize
    );

    BASE64_API bool decode(
        const char* input,
        std::size_t inputSize,
        std::uint8_t* output,
        std::size_t outputCapacity,
        std::size_t* outputSize
    );

    BASE64_API bool isValid(
        const char* input,
        std::size_t inputSize
    );

}
