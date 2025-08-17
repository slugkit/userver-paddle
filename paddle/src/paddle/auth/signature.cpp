#include <paddle/auth/signature.hpp>

#include <userver/crypto/hash.hpp>

#include <fmt/format.h>

#include <ctime>

namespace paddle {

// https://developer.paddle.com/webhooks/signature-verification#verify-manually
// The signature contains the timestamp and the signature itself
// ts=<timestamp>;h1=<signature>
// actually, timestamp is the data between first '=' and first ';'
// signature is the data after second '='
// The payload is the body of the request
// Signed payload is <timestamp>:<payload>
// Signature is HMAC-SHA256(secret, signed payload)
bool VerifySignature(
    std::string_view secret,
    std::string_view signature_header,
    std::string_view payload,
    std::int32_t max_age_seconds
) {
    auto first_equal = signature_header.find('=');
    if (first_equal == std::string::npos) {
        return false;
    }
    auto first_semicolon = signature_header.find(';');
    if (first_semicolon == std::string::npos) {
        return false;
    }
    std::string_view timestamp{signature_header.data() + first_equal + 1, signature_header.data() + first_semicolon};

    if (max_age_seconds >= 0) {
        const auto now = std::time(nullptr);
        const auto ts = std::stoi(std::string{timestamp});
        if (now - ts > max_age_seconds) {
            return false;
        }
    }

    auto second_equal = signature_header.find('=', first_semicolon + 1);
    if (second_equal == std::string::npos) {
        return false;
    }
    std::string_view signature{
        signature_header.data() + second_equal + 1, signature_header.data() + signature_header.size()
    };
    auto signed_payload = fmt::format("{}:{}", timestamp, payload);
    auto calculated_signature = userver::crypto::hash::HmacSha256(secret, signed_payload);
    return signature == calculated_signature;
}

}  // namespace paddle
