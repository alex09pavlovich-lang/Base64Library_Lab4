
#include "base64.h"

#include <iostream>
#include <string>
#include <vector>

int main() {

    std::string text = "Hello, Base64!";

    std::cout << "Original text: " << text << '\n';

    // Кодування
    std::vector<char> encoded(encodedSize(text.size()));

    std::size_t encodedLength = 0;

    bool encodeResult = encode(
        reinterpret_cast<const std::uint8_t*>(text.data()),
        text.size(),
        encoded.data(),
        encoded.size(),
        &encodedLength
    );

    if (!encodeResult) {
        std::cout << "Encoding error!\n";
        return 1;
    }

    std::string encodedText(encoded.data(), encodedLength);

    std::cout << "Encoded text: " << encodedText << '\n';

    // Перевірка правильності Base64
    std::cout << "Is valid: "
        << (isValid(encodedText.data(), encodedText.size())
            ? "Yes" : "No") << '\n';

    // Декодування
    std::vector<std::uint8_t> decoded(text.size());

    std::size_t decodedLength = 0;

    bool decodeResult = decode(
        encodedText.data(),
        encodedText.size(),
        decoded.data(),
        decoded.size(),
        &decodedLength
    );

    if (!decodeResult) {
        std::cout << "Decoding error!\n";
        return 1;
    }

    std::string decodedText(
        reinterpret_cast<const char*>(decoded.data()),
        decodedLength
    );

    std::cout << "Decoded text: " << decodedText << '\n';

    // Перевірка розміру
    std::cout << "Encoded size: "
        << encodedSize(text.size()) << '\n';

    return 0;
}
