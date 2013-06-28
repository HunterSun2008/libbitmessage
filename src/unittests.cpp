#include <stdint.h>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <ctime>
#include <botan/botan.h>
#include <botan/rng.h>
#include "utils.h"
#include "ecc.h"
#include "hashes.h"
#include "address.h"
#include "bitmessage.h"
#include "unittests.h"

using namespace std;

static void test_encode_hex()
{
    cout << "\n=== TEST ENCODE HEX ===\n\n";

    bm::ByteVector v1;
    v1.push_back(0x01);
    v1.push_back(0x02);
    v1.push_back(0x03);
    string hex = bm::utils::encode_hex(v1);
    cout << "123 as hex: " << hex << "\n";
    assert(hex == "010203");
    bm::ByteVector v2 = bm::utils::decode_hex(hex);
    assert(v2[0] == 0x01);
    assert(v2[1] == 0x02);
    assert(v2[2] == 0x03);

    cout << "\n=== OK ===\n" << endl;
}

static void test_serialize_varint()
{
    cout << "\n=== TEST ENCODE VARINT ===\n\n";

    int nb;
    uint64_t i1 = 123;
    bm::ByteVector v1 = bm::utils::serialize_varint(i1);
    string s = bm::utils::encode_hex(v1);
    cout << "123 (encoded): " << s << "\n";
    uint64_t i2 = bm::utils::deserialize_varint(v1, nb);
    cout << "123 (decoded): " << i2 << "\n";
    assert(s == "7B");
    assert(i1 == i2);

    i1 = 1234;
    s = bm::utils::encode_hex(bm::utils::serialize_varint(i1));
    cout << "1234: " << s << "\n";
    assert(s == "FD04D2");

    i1 = 66666;
    s = bm::utils::encode_hex(bm::utils::serialize_varint(i1));
    cout << "66666: " << bm::utils::encode_hex(bm::utils::serialize_varint(i1)) << "\n";
    assert(s == "FE0001046A");

    // FIXME: encode_hex is slow
    i1 = 4595967296;
    v1 = bm::utils::serialize_varint(i1);
    s = bm::utils::encode_hex(v1);
    cout << "4595967296 (encoded): " << s << "\n";
    i2 = bm::utils::deserialize_varint(v1, nb);
    cout << "4595967296 (decoded): " << i2 << "\n";
    assert(s == "FF0000000111F0E540");
    assert(i1 == i2);

    cout << "\n=== OK ===\n" << endl;
}

static void test_base58()
{
    cout << "\n=== TEST BASE58 ===\n\n";

    Botan::BigInt bi1(1234567890);
    string s = bm::utils::encode_base58(bi1);
    cout << "1234567890 (encoded): " << s << "\n";
    Botan::BigInt bi2 = bm::utils::decode_base58(s);    
    assert(bi1 == bi2);

    cout << "\n=== OK ===\n" << endl;
}

static void test_base64()
{
    cout << "\n=== TEST BASE64 ===\n\n";

    string s = "This is a string"; // VGhpcyBpcyBhIHN0cmluZw==
    bm::ByteVector v1((unsigned char*)&s.c_str()[0], s.length());
    string str = bm::utils::encode_base64(v1);
    cout << "\"This is a string\" (encoded): " << str << "\n";
    assert(str == "VGhpcyBpcyBhIHN0cmluZw==");
    bm::ByteVector v2 = bm::utils::decode_base64(str);
    assert(v1 == v2);

    cout << "\n=== OK ===\n" << endl;
}

static void test_ecc_keys()
{
    cout << "\n=== TEST ECC ===\n\n";

    cout << "=== Using password...\n";
    bm::ECC ecc;
    ecc.generate_keys_with_password("qwerty");
    cout << ecc.get_private_key() << "\n" << ecc.get_public_key() << "\n";

    cout << "=== Without password...\n";
    ecc.generate_keys();
    cout << ecc.get_private_key() << "\n" << ecc.get_public_key();

    cout << "\n=== OK ===" << endl;
}

static void test_ripemd160()
{
    cout << "\n=== TEST RIPEMD160 ===\n\n";

    string str = "This is a string"; // 291850ad6a9a191487f01b5fbe19c215de1a5d67
    bm::ByteVector v1 = bm::hash::ripemd160(str);
    bm::ByteVector v2 = bm::hash::ripemd160(str);
    cout << bm::utils::encode_hex(v1) << "\n";
    assert(v1.size() == 20);
    assert(v1 == v2);
    assert(v1[0] == 0x29);
    assert(v1[19] == 0x67);

    cout << "\n=== OK ===" << endl;
}

static void test_sha256()
{
    cout << "\n=== TEST SHA256 ===\n\n";

    string str = "This is a string"; // 4E9518575422C9087396887CE20477AB5F550A4AA3D161C5C22A996B0ABB8B35
    bm::ByteVector v1 = bm::hash::sha256(str);
    bm::ByteVector v2 = bm::hash::sha256(str);
    cout << bm::utils::encode_hex(v1) << "\n";
    assert(v1.size() == 32);
    assert(v1 == v2);
    assert(v1[0] == 0x4E);    
    assert(v1[31] == 0x35);

    cout << "\n=== OK ===" << endl;
}

static void test_sha512()
{
    cout << "\n=== TEST SHA512 ===\n\n";

    string str = "This is a string"; // F4D54D32E3523357FF023903EABA2721E8C8CFC7702663782CB3E52FAF2C56C002CC3096B5F2B6DF870BE665D0040E9963590EB02D03D166E52999CD1C430DB1
    bm::ByteVector v1 = bm::hash::sha512(str);
    bm::ByteVector v2 = bm::hash::sha512(str);
    cout << bm::utils::encode_hex(v1) << "\n";
    assert(v1.size() == 64);
    assert(v1 == v2);
    assert(v1[0] == 0xF4);    
    assert(v1[63] == 0xB1);

    cout << "\n=== OK ===\n" << endl;
}

static void test_hmac_sha256()
{
    cout << "\n=== TEST HMAC_SHA256 ===\n\n";

    Botan::AutoSeeded_RNG rng;
    bm::ByteVector key = rng.random_vec(32);
    bm::ByteVector data = rng.random_vec(1024);
    bm::ByteVector mac = bm::hash::hmac_sha256(data, key); // FIXME: Make a _real_ test
    bm::OctetVector ostr(mac);
    cout << ostr.as_string() << endl;
    // FIXME: Sanity checks

    cout << "\n=== OK ===\n" << endl;
}

static void test_hmac_sha512()
{
    cout << "\n=== TEST HMAC_SHA512 ===\n\n";

    Botan::AutoSeeded_RNG rng;
    bm::ByteVector key = rng.random_vec(32);
    bm::ByteVector data = rng.random_vec(1024);
    bm::ByteVector mac = bm::hash::hmac_sha512(data, key); // FIXME: Make a _real_ test
    bm::OctetVector ostr(mac);
    cout << ostr.as_string() << endl;
    // FIXME: Sanity checks

    cout << "\n=== OK ===\n" << endl;
}

static void test_addresses()
{
    cout << "\n=== TEST ADDRESSES ===\n\n";

    std::string addr = bm::address::create();
    cout << "Random address: " << addr << "\n";
    bm::address::remove_prefix(addr);
    string test = addr;
    cout << "Without prefix 1: " << addr << "\n";    
    bm::address::remove_prefix(addr);    
    cout << "Without prefix 2: " << addr << "\n";
    assert(test == addr);
    bm::address::add_prefix(addr);
    test = addr;
    cout << "With prefix 1: " << addr << "\n";
    bm::address::add_prefix(addr);
    cout << "With prefix 2: " << addr << "\n";
    assert(test == addr);

    cout << "\n=== OK ===\n" << endl;
}

void run_unit_tests()
{
    test_encode_hex();
    test_serialize_varint();
    test_base58();
    test_base64();
    test_ecc_keys();
    test_ripemd160();
    test_sha256();
    test_sha512();
    test_hmac_sha256();
    test_hmac_sha512();    
    test_addresses();
}
