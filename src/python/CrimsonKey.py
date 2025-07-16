import common
import argparse
import os

def check_key_length(key: bytes, min_length: int = 2) -> bool:
    """Check if the key meets requirements."""
    if len(key) < min_length:
        common.warn(f"Key is too short! Minimum length is {min_length} bytes, got {len(key)} bytes")
        return False
    return True

def encrypt_key(key: bytes) -> bytes:
    xor_val = os.urandom(1)[0]
    encr_key = os.urandom(1)[0]
    hint_byte = xor_val

    full_key = bytearray([hint_byte]) + key
    encrypted_key = bytearray(b ^ encr_key for b in full_key)
    return bytes([hint_byte]) + encrypted_key

def process_key(key_input: str):
    """Process a single key input and encrypt it."""
    try:
        clear_key = bytes(key_input, "ascii")
        
        if not check_key_length(clear_key):
            return
            
        common.okay(f"Key to encode: {clear_key}")
        common.info("Encrypting key")
        crimson_key = encrypt_key(clear_key)
        
        common.info(f"Hint byte: 0x{crimson_key[0]:02x}")
        
        key_bytes = ", ".join(f"0x{b:02x}" for b in crimson_key)
        common.okay(f"Encrypted key is: {key_bytes}")
        
    except UnicodeEncodeError:
        common.warn("Key contains non-ASCII characters. Please use ASCII characters only.")

def run_interactive():
    """Run the program in interactive mode."""
    common.info("Crimson Key Encoder - Interactive Mode")
    common.info("Enter keys to encrypt, or type 'exit', 'quit', or 'q' to stop")
    print()
    
    while True:
        try:
            key_input = input("Enter key to encrypt: ").strip()
            
            # Check for exit 
            if key_input.lower() in ['exit', 'quit', 'q',':q!','qa!']:
                common.info("Goodbye!")
                break
                
            if not key_input:
                common.warn("Empty input. Please enter a key or 'exit' to quit.")
                continue
                
            process_key(key_input)
            print()  
            
        except KeyboardInterrupt:
            print()  
            common.info("Interrupted by user. Goodbye!")
            break
        except EOFError:
            print()  
            common.info("End of input. Goodbye!")
            break

def parse():
    parser = argparse.ArgumentParser(
        description = "Encode your key to be later decoded using brute force - no plain keys in my binary",
        epilog="""
        
        Interactive mode (default):
        python crimsonkey.py
        
        Single key mode:
        python crimsonkey.py --key <input your key>

        """)
    
    parser.add_argument("--key", default = None, help="Input your encryption key (if not provided, runs in interactive mode)" )
    args = parser.parse_args()
    
    if args.key is None:
        print(
            r"""
            
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
             """
        )
        run_interactive()
    else:
        
        process_key(args.key)

if __name__ == "__main__":
    try:

        
        parse()
    except Exception as e:
        common.warn(f"Something went wrong, the error is {e}")