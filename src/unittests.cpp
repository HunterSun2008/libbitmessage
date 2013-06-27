//#include <stdio.h>
//#include <stdlib.h>
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

static void test_ecc_keys()
{
    cout << "\n=== TEST ECC ===\n";
    cout << "\n=== Using password...\n";
    bm::ECC ecc;
    ecc.generate_keys_with_password("qwerty");
    cout << ecc.get_private_key() << "\n" << ecc.get_public_key();
    cout << "\n=== Without password...\n";
    ecc.generate_keys();
    cout << ecc.get_private_key() << "\n" << ecc.get_public_key();
    cout << "\n=== TEST ECC OK ===" << endl;
}

static void test_ripemd160()
{
    cout << "\n=== TEST RIPEMD160 ===\n";
    string str = "This is a string"; // 291850ad6a9a191487f01b5fbe19c215de1a5d67
    bm::ByteVector v1 = bm::ripemd160(str);
    bm::ByteVector v2 = bm::ripemd160(str);
    assert(v1.size() == 20);
    assert(v1 == v2);
    assert(v1[0] == 0x29);
    assert(v1[19] == 0x67);
    cout << "\n=== TEST RIPEMD160 OK ===" << endl;
}

static void test_sha256()
{
    cout << "\n=== TEST SHA256 ===\n";
    string str = "This is a string"; // 4E9518575422C9087396887CE20477AB5F550A4AA3D161C5C22A996B0ABB8B35
    bm::ByteVector v1 = bm::sha256(str);
    bm::ByteVector v2 = bm::sha256(str);
    assert(v1.size() == 32);
    assert(v1 == v2);
    assert(v1[0] == 0x4E);    
    assert(v1[31] == 0x35);
    cout << "\n=== TEST SHA256 OK ===" << endl;
}

static void test_sha512()
{
    cout << "\n=== TEST SHA512 ===\n";
    string str = "This is a string"; // F4D54D32E3523357FF023903EABA2721E8C8CFC7702663782CB3E52FAF2C56C002CC3096B5F2B6DF870BE665D0040E9963590EB02D03D166E52999CD1C430DB1
    bm::ByteVector v1 = bm::sha512(str);
    bm::ByteVector v2 = bm::sha512(str);
    assert(v1.size() == 64);
    assert(v1 == v2);
    assert(v1[0] == 0xF4);    
    assert(v1[63] == 0xB1);
    cout << "\n=== TEST SHA512 OK ===\n" << endl;
}

static void test_hmac_sha256()
{
    cout << "\n=== TEST HMAC_SHA256 ===\n";
    Botan::AutoSeeded_RNG rng;
    bm::ByteVector key = rng.random_vec(32);
    bm::ByteVector data = rng.random_vec(1024);
    bm::ByteVector mac = bm::hmac_sha256(data, key); // FIXME: Make a _real_ test
    bm::OctetVector ostr(mac);
    cout << ostr.as_string() << endl;
    cout << "\n=== TEST HMAC_SHA256 OK ===\n" << endl;
}

static void test_hmac_sha512()
{
    cout << "\n=== TEST HMAC_SHA512 ===\n";
    Botan::AutoSeeded_RNG rng;
    bm::ByteVector key = rng.random_vec(32);
    bm::ByteVector data = rng.random_vec(1024);
    bm::ByteVector mac = bm::hmac_sha512(data, key); // FIXME: Make a _real_ test
    bm::OctetVector ostr(mac);
    cout << ostr.as_string() << endl;
    cout << "\n=== TEST HMAC_SHA512 OK ===\n" << endl;
}

static void test_addresses()
{
    cout << "\n=== TEST addresses ===\n";
    bm::create_random_address();
}

/*
static void test_pack()
{	
	string packed;
	unsigned long long v;

	printf("test_pack...");

    packed = utils::pack<unsigned char>((unsigned char)'j');
	assert(packed.length()==sizeof(unsigned char));
	assert(packed.substr(0,1).c_str()[0]=='j');

    packed = utils::pack<unsigned short>((unsigned short)1234);
    assert(packed.length()==sizeof(unsigned short));
    v = utils::unpack<unsigned short>(packed);
    assert(v == 1234);

    packed = utils::pack<unsigned int>((unsigned int)12345);
	assert(packed.length()==sizeof(unsigned int));
    v = utils::unpack<unsigned int>(packed);
	assert(v == 12345);

    packed = utils::pack<unsigned long long>((unsigned long long)1234567);
	assert(packed.length()==sizeof(unsigned long long));
    v = utils::unpack<unsigned long long>(packed);
	assert(v == 1234567);

	printf("Ok\n");
}

static void test_encodeVarint()
{
	string result;
	unsigned char firstByte;
	unsigned short vshort;
	unsigned int vint;
	unsigned long long vlong;
	int nbytes=0;

	printf("test_encodeVarint...");

	for (unsigned long long i = 0; i < 99999; i++) {
		result = bitmessage::encodeVarint(i);
		unsigned long long i2 = bitmessage::decodeVarint(result, &nbytes);
		if (i != i2) {
			printf("%lld %lld\n",i,i2);
		}
		assert(i==i2);
	}

	result = bitmessage::encodeVarint((unsigned char)123);
	assert(result.length() == 1);

	result = bitmessage::encodeVarint((unsigned short)1234);
	assert(result.length() == 3);
	assert(result.substr(0,1).c_str()[0] == (char)253);
	memcpy((void*)&vshort,(void*)result.substr(1,2).c_str(),2);
	assert(vshort == 1234);

	result = bitmessage::encodeVarint((unsigned int)68541);
	assert(result.length() == 5);
	assert(result.substr(0,1).c_str()[0] == (char)254);
	memcpy((void*)&vint,(void*)result.substr(1,4).c_str(),4);
	assert(vint == 68541);

	result = bitmessage::encodeVarint((unsigned long long)5294967296);
	assert(result.length() == 9);
	assert(result.substr(0,1).c_str()[0] == (char)255);
	memcpy((void*)&vlong,(void*)result.substr(1,8).c_str(),8);
	assert(vlong == 5294967296);

	printf("Ok\n");
}

static void test_proofOfWork()
{
	string payload;
	string embeddedTime = "1/2/3";
	string cyphertext = "This is a test";
	unsigned int payloadLengthExtraBytes=14000;
	unsigned int averageProofOfWorkNonceTrialsPerByte=320;
	clock_t begin_time, end_time;

	printf("test_proofOfWork...");

	begin_time = clock();
	payload = bitmessage::proofOfWork(1, embeddedTime,
									  cyphertext,
									  payloadLengthExtraBytes,
									  averageProofOfWorkNonceTrialsPerByte,
									  false);
	end_time = clock();
	int seconds = (int)((end_time-begin_time)/CLOCKS_PER_SEC);
	assert(seconds < 30);

    // proof of work should be correct
	assert(bitmessage::checkProofOfWork(payload,
										payloadLengthExtraBytes,
										averageProofOfWorkNonceTrialsPerByte));

	// if the proof of work is not correct
	unsigned long long real_nonce = 0;
	memcpy((void*)&real_nonce,(void*)payload.c_str(),8);
	char str[8];
	string s;
	string message_payload = payload.substr(8);
	for (unsigned long long bogus_nonce = 0; bogus_nonce < 100; bogus_nonce++) {
		if (real_nonce == bogus_nonce) break;
		memcpy((void*)str,(void*)&bogus_nonce,8);
		s = "";
		for (int i = 0; i < 8; i++) {
			s += str[i];
		}
		// the check should return false
		assert(!bitmessage::checkProofOfWork(s + message_payload,
											 payloadLengthExtraBytes,
											 averageProofOfWorkNonceTrialsPerByte));
	}
	
	printf("Ok\n");
}

static void test_base58()
{
	printf("test_base58...");

	for (unsigned int num = 0; num < 10000; num++) {
		string encoded = utils::encodeBase58(num);
		unsigned int decoded = utils::decodeBase58(encoded);
		assert(decoded == num);
	}

	printf("Ok\n");
}

static void test_address_encoding()
{
	printf("test_address_encoding...");

	string data = "01234567890123456789";

	for (unsigned int version = 2; version <=2; version++) {
		for (unsigned int streamNumber = 1; streamNumber < 300; streamNumber++) {
			string encoded = bitmessage::encodeAddress(version, streamNumber, data);

			assert(encoded.length() > 10);
			assert(encoded.substr(0,3)=="BM-");

			string decoded_status="";
			string decoded_data="";
			unsigned int decoded_version=0;
			unsigned int decoded_streamNumber=0;
			bitmessage::decodeAddress(encoded,
									  decoded_status,
									  decoded_data,
									  decoded_version,
									  decoded_streamNumber);
			if (decoded_status != "success") {
				printf("\nVersion %d   Stream %d\n", version, streamNumber);
				printf("Decode failed with status: %s\n",decoded_status.c_str());
			}
			assert(decoded_status == "success");
			if (decoded_version != version) {
				printf("\nVersion %d -> %d\n", version, decoded_version);
			}
			assert(decoded_version == version);
			if (decoded_streamNumber != streamNumber) {
				printf("\nstreamNumber %d -> %d\n", streamNumber, decoded_streamNumber);
			}
			assert(decoded_streamNumber == streamNumber);
		}
	}
	printf("Ok\n");
}

static void test_hex()
{
	printf("test_hex...");
	
	string original = "78910abcdefghijk";
	string encoded = utils::encodeHex(original);
	string decoded = utils::decodeHex(encoded);
	if (decoded != original) {
		printf("\nString\n  original %s\n  encoded %s\n  decoded %s\n",
			   original.c_str(), encoded.c_str(), decoded.c_str());
	}
	assert(encoded != decoded);
	assert(decoded == original);

	unsigned int num = 4539261;
	unsigned int decodedInt;
	encoded = utils::encodeHex(num);
	decodedInt = utils::decodeHexInt(encoded);
	if (decodedInt != num) {
		printf("\nInteger\n  num %d\n  encoded %s\n  decoded %d\n", num, encoded.c_str(), decodedInt);
	}
	assert(decodedInt == num);

	mpz_t decodedIntBig, numBig;
	mpz_init(decodedIntBig);
	mpz_init(numBig);
	string numStr = "12345677893347733782276";
	mpz_set_str(numBig,numStr.c_str(),10);
	encoded = utils::encodeHex(numBig);
	utils::decodeHexInt(encoded, decodedIntBig);
	char decodedStr[256];
	mpz_get_str(decodedStr,10,decodedIntBig);
	assert(strcmp(decodedStr,numStr.c_str())==0);

	printf("Ok\n");
}

static void test_extract_stream_number()
{
	printf("test_extract_stream_number...");

	string data = "01234567890123456789";
	string status="";

    for (unsigned int version = 2; version <= 2; version++) {
        for (unsigned int streamNumber = 1; streamNumber < 300; streamNumber++) {

            string encoded = bitmessage::encodeAddress(version, streamNumber, data);

            assert(encoded.length() > 10);

            unsigned int decoded_streamNumber = bitmessage::addressStreamNumber(encoded, status);
            if (decoded_streamNumber != streamNumber) {
                printf("\n%d %d\n",streamNumber,decoded_streamNumber);
            }
            assert(decoded_streamNumber == streamNumber);

            if (status != "success") {
                printf("\nVersion %d   Stream %d\n", version, streamNumber);
                printf("Decode failed with status: %s\n",status.c_str());
            }
            assert(status == "success");
        }
	}

	printf("Ok\n");
}
*/
void run_unit_tests()
{
    test_ecc_keys();
    test_ripemd160();
    test_sha256();
    test_sha512();
    test_hmac_sha256();
    test_hmac_sha512();
    test_addresses();
    //test_hex();    
    //test_pack();
    //test_encodeVarint();
    //test_base58();
    //test_address_encoding();
    //test_extract_stream_number();
    //test_proofOfWork();
}
