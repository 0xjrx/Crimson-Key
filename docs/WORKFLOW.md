# Crimson Key Workflow Guide

Step-by-step guide for using Crimson Key in your projects.

## Overview

Crimson Key provides a two-step workflow:
1. **Encode** your plaintext keys using Python
2. **Decode** them in your C/C++ application

## Step 1: Encoding Keys (Python)

### Installation
No installation required! Just ensure you have Python 3.6+.

### Basic Usage

#### Interactive Mode (Recommended)
```bash
cd src/python
python CrimsonKey.py
```

This starts an interactive session:
```
...
[*] Crimson Key Encoder - Interactive Mode
[*] Enter keys to encrypt, or type 'exit', 'quit', or 'q' to stop

Enter key to encrypt: mypassword
[+] Key to encode: b'mypassword'
[*] Encrypting key...
[*] Hint byte: 0x94
[+] Encrypted key is: 0x94, 0xf7, 0x2b, 0x02, 0x0f, 0x0f, 0x0c, 0x15, 0x17, 0x1a

Enter key to encrypt: exit
[*] Goodbye!
```

#### Single Key Mode
```bash
cd src/python
python CrimsonKey.py --key "mypassword"
```

Output:
```
[+] Key to encode: b'mypassword'
[*] Encrypting key...
[*] Hint byte: 0x94
[+] Encrypted key is: 0x94, 0xf7, 0x2b, 0x02, 0x0f, 0x0f, 0x0c, 0x15, 0x17, 0x1a
```

### Key Requirements
- **Minimum length**: 2 bytes (configurable)
- **Character set**: ASCII only
- **Recommended**: 8+ characters for better security

### Understanding the Output
The output format is ready for C arrays:
```
0x94, 0xf7, 0x2b, 0x02, 0x0f, 0x0f, 0x0c, 0x15, 0x17, 0x1a
```

- **First byte (0x94)**: Hint byte used for brute force recovery
- **Remaining bytes**: XOR-encrypted key data

## Step 2: Decoding Keys (C/C++)

### Setup
1. Copy `src/c-api/crimsonkey_api.h` to your project
2. Include it in your source files

### Basic Usage

#### Simple Decryption
```c
#include "crimsonkey_api.h"

int main() {
    // Paste the bytes from Python output
    uint8_t encrypted_key[] = { 0x94, 0xf7, 0x2b, 0x02, 0x0f, 0x0f, 0x0c, 0x15, 0x17, 0x1a };
    
    uint8_t *decrypted_key = NULL;
    size_t decrypted_length = 0;
    
    // Decrypt the key
    crimsonkey_result_t result = crimsonkey_decrypt(
        encrypted_key, 
        sizeof(encrypted_key), 
        &decrypted_key, 
        &decrypted_length,
        0  // Default flags
    );
    
    if (result == CRIMSONKEY_SUCCESS) {
        printf("Decrypted key: %.*s\n", (int)decrypted_length, decrypted_key);
        crimsonkey_free(decrypted_key);  // Always free!
    } else {
        printf("Decryption failed: %s\n", crimsonkey_get_error_string(result));
    }
    
    return 0;
}
```

#### Error Handling
```c
crimsonkey_result_t result = crimsonkey_decrypt(
    encrypted_data, length, &key, &keylen, CRIMSONKEY_SILENT
);

switch (result) {
    case CRIMSONKEY_SUCCESS:
        printf("Key: %.*s\n", (int)keylen, key);
        crimsonkey_free(key);
        break;
        
    case CRIMSONKEY_ERROR_INVALID_LENGTH:
        printf("Invalid encrypted data\n");
        break;
        
    case CRIMSONKEY_ERROR_DECRYPTION_FAILED:
        printf("Could not decrypt - data may be corrupted\n");
        break;
        
    default:
        printf("Error: %s\n", crimsonkey_get_error_string(result));
        break;
}
```

## Real-World Use Cases

### 1. Configuration Files
Instead of storing plain passwords:

**Bad:**
```ini
[database]
password=mySecretPassword123
```

**Good:**
```ini
[database] 
encrypted_password=0x94,0xf7,0x2b,0x02,0x0f,0x0f,0x0c,0x15,0x17,0x1a
```

**C Code:**
```c
// Parse config and decrypt password
uint8_t encrypted[] = { 0x94, 0xf7, 0x2b, 0x02, 0x0f, 0x0f, 0x0c, 0x15, 0x17, 0x1a };
uint8_t *password = NULL;
size_t len = 0;

if (crimsonkey_decrypt(encrypted, sizeof(encrypted), &password, &len, CRIMSONKEY_SILENT) == CRIMSONKEY_SUCCESS) {
    // Use password for database connection
    connect_to_database((char*)password);
    crimsonkey_free(password);
}
```

### 2. Embedded Systems
Hide API keys in firmware:

```c
// In your embedded application
const uint8_t API_KEY_ENCRYPTED[] = { 0x94, 0xf7, 0x2b, 0x02, 0x0f, 0x0f, 0x0c };

void setup_api_connection() {
    uint8_t *api_key = NULL;
    size_t key_len = 0;
    
    if (crimsonkey_decrypt(API_KEY_ENCRYPTED, sizeof(API_KEY_ENCRYPTED), 
                          &api_key, &key_len, CRIMSONKEY_SILENT) == CRIMSONKEY_SUCCESS) {
        
        // Use API key for authentication
        authenticate_api((char*)api_key);
        
        // Clear sensitive data from memory
        memset(api_key, 0, key_len);
        crimsonkey_free(api_key);
    }
}
```

### 3. Source Code Protection
Avoid hardcoded secrets:

```c
// Instead of:
// const char* SECRET = "myHardcodedSecret";

// Use:
const uint8_t SECRET_ENC[] = { /* encrypted bytes */ };

char* get_secret() {
    uint8_t *secret = NULL;
    size_t len = 0;
    
    if (crimsonkey_decrypt(SECRET_ENC, sizeof(SECRET_ENC), &secret, &len, CRIMSONKEY_SILENT) == CRIMSONKEY_SUCCESS) {
        // Convert to null-terminated string
        char *result = malloc(len + 1);
        memcpy(result, secret, len);
        result[len] = '\0';
        
        crimsonkey_free(secret);
        return result;
    }
    return NULL;
}
```

## Build Integration

### CMake Example
```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Add Crimson Key include directory
include_directories(${CMAKE_SOURCE_DIR}/third_party/crimson-key/src/c-api)

add_executable(myapp main.c)
```

### Makefile Example
```makefile
# Makefile
CRIMSONKEY_INCLUDE = ./crimson-key/src/c-api
CFLAGS = -I$(CRIMSONKEY_INCLUDE) -std=c99

myapp: main.c
	gcc $(CFLAGS) -o myapp main.c
```

## Security Considerations

### What Crimson Key Provides
✅ **Obfuscation** - Hides keys from casual inspection  
✅ **Simple encoding** - Easy to integrate  
✅ **No dependencies** - Works anywhere  

### What Crimson Key Does NOT Provide
❌ **Cryptographic security** - Uses simple XOR  
❌ **Protection against reverse engineering** - Can be broken with analysis  
❌ **Key derivation** - Not suitable for password storage  

### Best Practices
1. **Use for obfuscation only** - Not for protecting high-value secrets
2. **Combine with other security measures** - Encryption, access controls, etc.
3. **Consider your threat model** - Appropriate for hiding from casual users
4. **Use longer keys** - 16+ characters recommended
5. **Clear sensitive memory** - Zero out decrypted keys when done

## Troubleshooting

### Common Issues

#### "Key is too short" Error
```bash
[-] Key is too short! Minimum length is 2 bytes, got 1 bytes
```
**Solution**: Use longer keys (8+ characters recommended)

#### "Decryption failed" in C
```c
crimsonkey_decrypt() returns CRIMSONKEY_ERROR_DECRYPTION_FAILED
```
**Possible causes**:
- Incorrect byte array (typo in copying from Python output)
- Data corruption during transfer
- Wrong array size

#### "Memory allocation failed"
```c
crimsonkey_decrypt() returns CRIMSONKEY_ERROR_MEMORY_ALLOCATION
```
**Solution**: Check available memory, especially on embedded systems

### Debugging Tips

1. **Use verbose mode** during development:
   ```c
   crimsonkey_decrypt(data, len, &key, &keylen, CRIMSONKEY_VERBOSE);
   ```

2. **Verify byte arrays** match Python output exactly

3. **Check array sizes**:
   ```c
   printf("Array size: %zu\n", sizeof(encrypted_array));
   ```

4. **Test with known good data** first

## Performance Notes

- **Decryption time**: O(256) maximum (brute force through XOR keys)
- **Memory usage**: Allocates space for decrypted key only
- **Typical performance**: < 1ms on modern hardware
- **Embedded systems**: Fast enough for most applications

## Next Steps

1. **Try the examples**: Build and run `examples/example_usage.c`
2. **Read the API docs**: See `docs/API.md` for complete reference
3. **Integrate into your project**: Copy the header file and start coding!
4. **Contribute**: Submit issues or improvements on GitHub

*"No AI agents were harmed during the making of this documentation."* 
