#include "../src/c-api/crimsonkey_api.h"

int main() {
    // Example encrypted key (you would get this from your Python script)
    uint8_t encrypted_key[] = { 0x3c, 0xf8, 0x8c, 0xa5, 0xa8, 0xa8, 0xab, 0x8c, 0xa5, 0xa8, 0xa8, 0xab };
    size_t encrypted_length = sizeof(encrypted_key);
    
    uint8_t *decrypted_key = NULL;
    size_t decrypted_length = 0;
    
    printf("=== Crimson Key API Example ===\n");
    printf("Encrypted data: ");
    for (size_t i = 0; i < encrypted_length; i++) {
        printf("0x%02X ", encrypted_key[i]);
    }
    printf("\n\n");
    
    // Decrypt the key with verbose output
    crimsonkey_result_t result = crimsonkey_decrypt(
        encrypted_key, 
        encrypted_length, 
        &decrypted_key, 
        &decrypted_length,
        CRIMSONKEY_VERBOSE
    );
    
    if (result == CRIMSONKEY_SUCCESS) {
        printf("\nDecrypted key: ");
        for (size_t i = 0; i < decrypted_length; i++) {
            printf("%c", decrypted_key[i]);
        }
        printf("\n");
        
        printf("Decrypted bytes: ");
        for (size_t i = 0; i < decrypted_length; i++) {
            printf("0x%02X ", decrypted_key[i]);
        }
        printf("\n");
        
        // Always free the allocated memory
        crimsonkey_free(decrypted_key);
    } else {
        printf("Error: %s\n", crimsonkey_get_error_string(result));
    }
    
    printf("\n=== Silent Mode Example ===\n");
    
    // Example with silent mode (no output messages)
    result = crimsonkey_decrypt(
        encrypted_key, 
        encrypted_length, 
        &decrypted_key, 
        &decrypted_length,
        CRIMSONKEY_SILENT
    );
    
    if (result == CRIMSONKEY_SUCCESS) {
        printf("Silent decryption successful: %.*s\n", (int)decrypted_length, decrypted_key);
        crimsonkey_free(decrypted_key);
    }
    
    return 0;
}
