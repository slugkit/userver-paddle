#include <paddle/auth/signature.hpp>

#include <userver/utest/utest.hpp>

namespace paddle {

namespace {

const auto kSecret = "pdl_ntfset_01k2jjfqx34sdwsvrbj123wxx2_w4C4I+q1LScX16/ODlt39IBfOQo+20fN";
const auto kPayload =
    R"({"data":{"id":"paymtd_01k2jj1kp58a2w0q2bz6868k7t","type":"card","origin":"subscription","saved_at":"2025-08-13T20:30:50.472214Z","address_id":"add_01k2jj0xcjspzt29qb3qtt5wx6","updated_at":"2025-08-13T20:40:50.668611Z","customer_id":"ctm_01k2jj0xbzdpzbgz0vqv1e0x5e","deletion_reason":"replaced_by_newer_version"},"event_id":"evt_01k2jjm0qdjr26zsz4m48z2efq","event_type":"payment_method.deleted","occurred_at":"2025-08-13T20:40:50.669839Z","notification_id":"ntf_01k2jjm13zz5m5t681nvn0e5hr"})";
const auto kSignatureHeader = "ts=1755117651;h1=cf519461c15c010f1a82e28afc83b7e8a5fdf1823791050e775badbe0bdcabf7";

}  // namespace

TEST(Paddle, Signature) {
    ASSERT_TRUE(VerifySignature(kSecret, kSignatureHeader, kPayload));
    ASSERT_FALSE(VerifySignature(
        kSecret, "ts=1755117661;h1=cf519461c15c010f1a82e28afc83b7e8a5fdf1823791050e775badbe0bdcabf7", kPayload
    ));
    ASSERT_FALSE(VerifySignature(kSecret, kSignatureHeader, kPayload, 0));
}

}  // namespace paddle