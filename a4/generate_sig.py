import sys
from cryptography.hazmat.primitives.asymmetric import ed25519
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend
from datetime import datetime

if len(sys.argv) != 2:
    print("Invalid args!")
    sys.exit(1)

private_key_path = sys.argv[1]

uid = private_key_path[-5:]

try:
    with open(private_key_path, "rb") as key_file:
        private_key = serialization.load_ssh_private_key(
            key_file.read(),
            password=None,
            backend=default_backend()
        )
except FileNotFoundError:
    print(f"Error: Private key for {uid} not found")
    sys.exit(1)

message = b"Authenticate"

signature = private_key.sign(message)

signature_file = f"signature_{uid}.bin"
with open(signature_file, "wb") as f:
    f.write(signature)

print(f"Signature for {uid} saved to {signature_file}")
