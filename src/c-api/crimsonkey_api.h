#ifndef CRIMSONKEY_API_H
#define CRIMSONKEY_API_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// API return codes
typedef enum {
    CRIMSONKEY_SUCCESS = 0,
    CRIMSONKEY_ERROR_NULL_POINTER = -1,
    CRIMSONKEY_ERROR_INVALID_LENGTH = -2,
    CRIMSONKEY_ERROR_MEMORY_ALLOCATION = -3,
    CRIMSONKEY_ERROR_DECRYPTION_FAILED = -4
} crimsonkey_result_t;

// Configuration flags
#define CRIMSONKEY_SILENT    0x01  // Suppress output messages
#define CRIMSONKEY_VERBOSE   0x02  // Enable verbose output


crimsonkey_result_t crimsonkey_decrypt(
    const uint8_t *encrypted_data, 
    size_t encrypted_length,
    uint8_t **decrypted_key,
    size_t *decrypted_length,
    uint32_t flags
);

void crimsonkey_free(uint8_t *ptr);

const char* crimsonkey_get_error_string(crimsonkey_result_t error_code);

// print helper macros
#define _CRIMSONKEY_OKAY(flags, msg, ...) \
    do { if (!(flags & CRIMSONKEY_SILENT)) printf("[+] " msg "\n", ##__VA_ARGS__); } while(0)
#define _CRIMSONKEY_WARN(flags, msg, ...) \
    do { if (!(flags & CRIMSONKEY_SILENT)) fprintf(stderr, "[-] " msg "\n", ##__VA_ARGS__); } while(0)
#define _CRIMSONKEY_INFO(flags, msg, ...) \
    do { if (!(flags & CRIMSONKEY_SILENT)) printf("[*] " msg "\n", ##__VA_ARGS__); } while(0)

// Implementation
crimsonkey_result_t crimsonkey_decrypt(
    const uint8_t *encrypted_data, 
    size_t encrypted_length,
    uint8_t **decrypted_key,
    size_t *decrypted_length,
    uint32_t flags
) {
    // Input validation
    if (!encrypted_data || !decrypted_key || !decrypted_length) {
        return CRIMSONKEY_ERROR_NULL_POINTER;
    }
    
    if (encrypted_length < 2) {
        _CRIMSONKEY_WARN(flags, "Invalid encrypted data length: %zu (minimum 2 bytes required)", encrypted_length);
        return CRIMSONKEY_ERROR_INVALID_LENGTH;
    }

    _CRIMSONKEY_INFO(flags, "Brute-forcing key decryption...");
    
    uint8_t hint = encrypted_data[0];
    *decrypted_key = NULL;
    *decrypted_length = 0;

    for (int i = 0; i < 256; ++i) {
        uint8_t xor_candidate = (uint8_t)i;

        if ((encrypted_data[1] ^ xor_candidate) == hint) {
            // Found correct key
            size_t real_len = encrypted_length - 2;  // Remove hint and encrypted hint
            *decrypted_key = (uint8_t*)malloc(real_len);
            if (!*decrypted_key) {
                _CRIMSONKEY_WARN(flags, "Memory allocation failed");
                return CRIMSONKEY_ERROR_MEMORY_ALLOCATION;
            }

            for (size_t j = 0; j < real_len; ++j) {
                (*decrypted_key)[j] = encrypted_data[j + 2] ^ xor_candidate;
            }

            *decrypted_length = real_len;
            _CRIMSONKEY_OKAY(flags, "Decrypted key with hint 0x%02X", hint);
            return CRIMSONKEY_SUCCESS;
        }
    }
    
    _CRIMSONKEY_WARN(flags, "Failed to decrypt key - no valid XOR candidate found");
    return CRIMSONKEY_ERROR_DECRYPTION_FAILED;
}

void crimsonkey_free(uint8_t *ptr) {
    if (ptr) {
        free(ptr);
    }
}

const char* crimsonkey_get_error_string(crimsonkey_result_t error_code) {
    switch (error_code) {
        case CRIMSONKEY_SUCCESS:
            return "Success";
        case CRIMSONKEY_ERROR_NULL_POINTER:
            return "Null pointer provided";
        case CRIMSONKEY_ERROR_INVALID_LENGTH:
            return "Invalid data length";
        case CRIMSONKEY_ERROR_MEMORY_ALLOCATION:
            return "Memory allocation failed";
        case CRIMSONKEY_ERROR_DECRYPTION_FAILED:
            return "Decryption failed";
        default:
            return "Unknown error";
    }
}

#ifdef __cplusplus
}
#endif

#endif // CRIMSONKEY_API_H