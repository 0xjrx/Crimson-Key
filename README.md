# Crimson Key

```
 ▄████████    ▄████████  ▄█    ▄▄▄▄███▄▄▄▄      ▄████████  ▄██████▄  ███▄▄▄▄      ▄█   ▄█▄    ▄████████ ▄██   ▄   
███    ███   ███    ███ ███  ▄██▀▀▀███▀▀▀██▄   ███    ███ ███    ███ ███▀▀▀██▄   ███ ▄███▀   ███    ███ ███   ██▄ 
███    █▀    ███    ███ ███▌ ███   ███   ███   ███    █▀  ███    ███ ███   ███   ███▐██▀     ███    █▀  ███▄▄▄███ 
███         ▄███▄▄▄▄██▀ ███▌ ███   ███   ███   ███        ███    ███ ███   ███  ▄█████▀     ▄███▄▄▄     ▀▀▀▀▀▀███ 
███        ▀▀███▀▀▀▀▀   ███▌ ███   ███   ███ ▀███████████ ███    ███ ███   ███ ▀▀█████▄    ▀▀███▀▀▀     ▄██   ███ 
███    █▄  ▀███████████ ███  ███   ███   ███          ███ ███    ███ ███   ███   ███▐██▄     ███    █▄  ███   ███ 
███    ███   ███    ███ ███  ███   ███   ███    ▄█    ███ ███    ███ ███   ███   ███ ▀███▄   ███    ███ ███   ███ 
████████▀    ███    ███ █▀    ▀█   ███   █▀   ▄████████▀   ▀██████▀   ▀█   █▀    ███   ▀█▀   ██████████  ▀█████▀  
             ███    ███                                                          ▀                                
                                                                                                        by 0xjrx
```

> "No plain text keys in my binary."  
> — *Plato, probably*

A dual-language cryptographic key encoding/decoding system with Python encoder and C decoder API.

## 🔑 What is Crimson Key?

Crimson Key is a security-focused tool that allows you to:
- **Encode** keys using Python with XOR encryption and brute-force hints
- **Decode** keys using a lightweight C API in your applications
- **Avoid storing plain keys** in binaries or configuration files

## 📁 Repository Structure

```
Crimson-Key/
├── src/
│   ├── python/              # Python key encoder
│   │   ├── CrimsonKey.py    # Main encoder script
│   │   └── common/          # Utility modules
│   └── c-api/               # C decoding API
│       └── crimsonkey_api.h # Header-only C library
├── examples/                # Usage examples
│   └── example_usage.c      # C API example
├── scripts/                 # Build and utility scripts
│   ├── build.sh            # Linux/macOS build script
│   └── build.bat           # Windows build script
├── docs/                    # Documentation
│   ├── API.md              # API reference
│   └── WORKFLOW.md         # Usage workflow
└── README.md               # This file
```

## 🚀 Quick Start

### 1. Encode a Key (Python)
```bash
cd src/python
python CrimsonKey.py --key "mypassword"
# Output: 0x94, 0xf7, 0x2b, 0x02, 0x0f, 0x0f, 0x0c
```

### 2. Decode in C
```c
#include "../../src/c-api/crimsonkey_api.h"

uint8_t encrypted[] = { 0x94, 0xf7, 0x2b, 0x02, 0x0f, 0x0f, 0x0c };
uint8_t *decrypted = NULL;
size_t length = 0;

if (crimsonkey_decrypt(encrypted, sizeof(encrypted), &decrypted, &length, 0) == CRIMSONKEY_SUCCESS) {
    printf("Decrypted: %.*s\n", (int)length, decrypted);
    crimsonkey_free(decrypted);
}
```

## 📚 Documentation

- [API Reference](docs/API.md) - Complete C API documentation
- [Workflow Guide](docs/WORKFLOW.md) - Step-by-step usage examples
- [Example Code](examples/) - Working examples in C

## 🛠️ Building Examples

### Windows:
```cmd
scripts\build.bat
```

### Linux/macOS:
```bash
chmod +x scripts/build.sh
./scripts/build.sh
```

## 🎯 Use Cases

- **Embedded systems** - Hide keys in firmware
- **Configuration files** - Obfuscate sensitive data
- **Source code** - Avoid plain text secrets
- **Reverse engineering protection** - Make key extraction harder

## 🔒 Security Notes

- Uses XOR encryption with brute-force recovery
- **Not cryptographically secure** - designed for obfuscation
- Minimum key length enforced (configurable)
- Suitable for hiding keys from casual inspection

## 📄 License

This project is open source. See LICENSE file for details.

## � Acknowledgments

This project was inspired by techniques learned from [Maldev Academy](https://maldevacademy.com/), specifically their module on key obfuscation. Special thanks to the Maldev Academy team for their excellent educational content on malware development and security research.

## �👨‍💻 Author

Created by **0xjrx**

---

*"No AI agents were harmed during the making of this documentation."* 
