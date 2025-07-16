# Crimson Key C API Reference

Documentation for the Crimson Key C decoding library.

## Overview

The Crimson Key C API is a header-only library that provides key decryption functionality for keys encoded with the Python Crimson Key tool.

## Quick Reference

```c
#include "crimsonkey_api.h"

// Decrypt a key
crimsonkey_result_t crimsonkey_decrypt(
    const uint8_t *encrypted_data, 
    size_t encrypted_length,
    uint8_t **decrypted_key,
    size_t *decrypted_length,
    uint32_t flags
);

// Free allocated memory
void crimsonkey_free(uint8_t *ptr);

// Get error description
const char* crimsonkey_get_error_string(crimsonkey_result_t error_code);
```

## Functions

### `crimsonkey_decrypt()`

Decrypts a Crimson Key encrypted key using brute force XOR recovery.

**Signature:**
```c
crimsonkey_result_t crimsonkey_decrypt(
    const uint8_t *encrypted_data, 
    size_t encrypted_length,
    uint8_t **decrypted_key,
    size_t *decrypted_length,
    uint32_t flags
);
```

**Parameters:**
- `encrypted_data`: Pointer to the encrypted key data
- `encrypted_length`: Length of the encrypted data in bytes
- `decrypted_key`: Output pointer for the decrypted key (caller must free with `crimsonkey_free()`)
- `decrypted_length`: Output length of the decrypted key
- `flags`: Configuration flags (see Flags section)

**Returns:** 
- `CRIMSONKEY_SUCCESS` on successful decryption
- Error code on failure (see Error Codes section)

**Example:**
```c
uint8_t encrypted[] = { 0x94, 0xf7, 0x2b, 0x02, 0x0f, 0x0f, 0x0c };
uint8_t *decrypted = NULL;
size_t length = 0;

crimsonkey_result_t result = crimsonkey_decrypt(
    encrypted, sizeof(encrypted), &decrypted, &length, 0
);

if (result == CRIMSONKEY_SUCCESS) {
    printf("Key: %.*s\n", (int)length, decrypted);
    crimsonkey_free(decrypted);
}
```

### `crimsonkey_free()`

Safely frees memory allocated by the Crimson Key library.

**Signature:**
```c
void crimsonkey_free(uint8_t *ptr);
```

**Parameters:**
- `ptr`: Pointer to memory allocated by `crimsonkey_decrypt()`

**Notes:**
- Safe to call with NULL pointer
- Must be used to free all pointers returned by `crimsonkey_decrypt()`

### `crimsonkey_get_error_string()`

Returns a human-readable description of an error code.

**Signature:**
```c
const char* crimsonkey_get_error_string(crimsonkey_result_t error_code);
```

**Parameters:**
- `error_code`: Error code returned by `crimsonkey_decrypt()`

**Returns:**
- Static string describing the error

**Example:**
```c
crimsonkey_result_t result = crimsonkey_decrypt(/*...*/);
if (result != CRIMSONKEY_SUCCESS) {
    printf("Error: %s\n", crimsonkey_get_error_string(result));
}
```

## Error Codes

```c
typedef enum {
    CRIMSONKEY_SUCCESS = 0,              // Operation successful
    CRIMSONKEY_ERROR_NULL_POINTER = -1,  // Null pointer provided
    CRIMSONKEY_ERROR_INVALID_LENGTH = -2,// Invalid data length
    CRIMSONKEY_ERROR_MEMORY_ALLOCATION = -3, // Memory allocation failed
    CRIMSONKEY_ERROR_DECRYPTION_FAILED = -4  // Decryption failed
} crimsonkey_result_t;
```

### Error Descriptions

- **`CRIMSONKEY_SUCCESS`**: Operation completed successfully
- **`CRIMSONKEY_ERROR_NULL_POINTER`**: One or more required pointers were NULL
- **`CRIMSONKEY_ERROR_INVALID_LENGTH`**: Encrypted data is too short (< 2 bytes)
- **`CRIMSONKEY_ERROR_MEMORY_ALLOCATION`**: Failed to allocate memory for decrypted key
- **`CRIMSONKEY_ERROR_DECRYPTION_FAILED`**: No valid XOR key found during brute force

## Configuration Flags

```c
#define CRIMSONKEY_SILENT    0x01  // Suppress output messages
#define CRIMSONKEY_VERBOSE   0x02  // Enable verbose output
```

### Flag Usage

**Default (0):**
```c
crimsonkey_decrypt(data, len, &key, &keylen, 0);
// Shows basic success/error messages
```

**Silent Mode:**
```c
crimsonkey_decrypt(data, len, &key, &keylen, CRIMSONKEY_SILENT);
// No console output
```

**Verbose Mode:**
```c
crimsonkey_decrypt(data, len, &key, &keylen, CRIMSONKEY_VERBOSE);
// Detailed progress messages
```

## Memory Management

The library allocates memory for decrypted keys using `malloc()`. **You must free this memory** using `crimsonkey_free()`:

```c
uint8_t *key = NULL;
size_t length = 0;

if (crimsonkey_decrypt(data, len, &key, &length, 0) == CRIMSONKEY_SUCCESS) {
    // Use the key...
    printf("Key: %.*s\n", (int)length, key);
    
    // Always free when done!
    crimsonkey_free(key);
}
```

## Thread Safety

The library is **thread-safe** as it:
- Uses no global variables
- Has no shared state between calls
- Only uses stack and heap memory

Multiple threads can safely call library functions simultaneously.

## Integration

### Header-Only Usage
1. Copy `crimsonkey_api.h` to your project
2. Include the header: `#include "crimsonkey_api.h"`
3. No linking required - everything is in the header

### Compiler Requirements
- **C99 or later** (uses `uint8_t`, `size_t`)
- **Standard library**: `stdio.h`, `stdlib.h`, `string.h`, `stdint.h`

### Platform Support
- **Windows** ✅
- **Linux** ✅  
- **macOS** ✅
- **Embedded systems** ✅ (with standard C library)

## Example Programs

See the `examples/` directory for complete working examples:
- `example_usage.c` - Basic usage demonstration
- More examples coming soon

## Best Practices

1. **Always check return values:**
   ```c
   if (crimsonkey_decrypt(/*...*/) != CRIMSONKEY_SUCCESS) {
       // Handle error
   }
   ```

2. **Always free allocated memory:**
   ```c
   crimsonkey_free(decrypted_key);
   ```

3. **Use appropriate flags:**
   ```c
   // Library code - use silent mode
   crimsonkey_decrypt(/*...*/, CRIMSONKEY_SILENT);
   
   // Debug builds - use verbose mode
   crimsonkey_decrypt(/*...*/, CRIMSONKEY_VERBOSE);
   ```

4. **Validate input data:**
   ```c
   if (encrypted_length < 2) {
       // Too short for valid Crimson Key data
   }
   ```
*"No AI agents were harmed during the making of this documentation."* 
