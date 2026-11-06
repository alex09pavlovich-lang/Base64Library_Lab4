
#include "base64.h"

#include <limits>

namespace {

    const char BASE64_TABLE[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    int decodeChar(char c) {
        if (c >= 'A' && c <= 'Z') return c - 'A';
        if (c >= 'a' && c <= 'z') return c - 'a' + 26;
        if (c >= '0' && c <= '9') return c - '0' + 52;
        if (c == '+') return 62;
        if (c == '/') return 63;

        return -1;
    }

}

std::size_t encodedSize(std::size_t inputSize) {
    const std::size_t max =
        std::numeric_limits<std::size_t>::max();

    if (inputSize > max - 2) {
        return max;
    }

    std::size_t groups = (inputSize + 2) / 3;

    if (groups > max / 4) {
        return max;
    }

    return groups * 4;
}


bool encode(
    const std::uint8_t* input,
    std::size_t inputSize,
    char* output,
    std::size_t outputCapacity,
    std::size_t* outputSize
) {
    if (outputSize == nullptr) return false;
    *outputSize = 0;

    if (input == nullptr && inputSize != 0) return false;

    std::size_t required = encodedSize(inputSize);

    if (required == std::numeric_limits<std::size_t>::max())
        return false;

    if (outputCapacity < required) return false;
    if (output == nullptr && required != 0) return false;

    std::size_t i = 0;
    std::size_t j = 0;

    while (i < inputSize) {
        std::size_t remaining = inputSize - i;

        std::uint32_t a = input[i++];
        std::uint32_t b = remaining > 1 ? input[i++] : 0;
        std::uint32_t c = remaining > 2 ? input[i++] : 0;

        std::uint32_t triple =
            (a << 16) | (b << 8) | c;

        output[j++] = BASE64_TABLE[(triple >> 18) & 0x3F];
        output[j++] = BASE64_TABLE[(triple >> 12) & 0x3F];

        output[j++] = remaining > 1
            ? BASE64_TABLE[(triple >> 6) & 0x3F]
            : '=';

        output[j++] = remaining > 2
            ? BASE64_TABLE[triple & 0x3F]
            : '=';
    }

    *outputSize = j;
    return true;
}


bool isValid(
    const char* input,
    std::size_t inputSize
) {
    if (input == nullptr && inputSize != 0) return false;

    if (inputSize % 4 != 0) return false;

    if (inputSize == 0) return true;

    std::size_t padding = 0;

    if (input[inputSize - 1] == '=') padding++;

    if (inputSize >= 2 && input[inputSize - 2] == '=') {
        padding++;
    }

    for (std::size_t i = 0; i < inputSize - padding; i++) {
        if (decodeChar(input[i]) == -1) return false;
    }

    for (std::size_t i = inputSize - padding; i < inputSize; i++) {
        if (input[i] != '=') return false;
    }

    if (padding > 2) return false;

    if (padding == 2) {
        if ((decodeChar(input[inputSize - 3]) & 0x0F) != 0)
            return false;
    }

    if (padding == 1) {
        if ((decodeChar(input[inputSize - 2]) & 0x03) != 0)
            return false;
    }

    return true;
}

bool decode(
    const char* input,
    std::size_t inputSize,
    std::uint8_t* output,
    std::size_t outputCapacity,
    std::size_t* outputSize
) {
    if (outputSize == nullptr) return false;
    *outputSize = 0;

    if (!isValid(input, inputSize)) return false;

    std::size_t padding = 0;

    if (inputSize > 0 && input[inputSize - 1] == '=') padding++;
    if (inputSize > 1 && input[inputSize - 2] == '=') padding++;

    std::size_t required = (inputSize / 4) * 3 - padding;

    if (outputCapacity < required) return false;
    if (output == nullptr && required != 0) return false;

    std::size_t j = 0;

    for (std::size_t i = 0; i < inputSize; i += 4) {
        std::uint32_t a = decodeChar(input[i]);
        std::uint32_t b = decodeChar(input[i + 1]);

        std::uint32_t c = input[i + 2] == '='
            ? 0 : decodeChar(input[i + 2]);

        std::uint32_t d = input[i + 3] == '='
            ? 0 : decodeChar(input[i + 3]);

        std::uint32_t triple =
            (a << 18) | (b << 12) | (c << 6) | d;

        if (j < required)
            output[j++] = (triple >> 16) & 0xFF;

        if (j < required)
            output[j++] = (triple >> 8) & 0xFF;

        if (j < required)
            output[j++] = triple & 0xFF;
    }

    *outputSize = j;
    return true;
}
