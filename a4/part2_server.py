from flask import Flask, jsonify, request
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import ed25519
from cryptography.hazmat.backends import default_backend

app = Flask(__name__)

ALLOWED_UIDS = {"test1", "test2", "test3", "test4", "test5"}
user_keys = {}
actions_log = []


@app.before_request
def log_request_info():
    print(f"Incoming request: {request.method} {request.path}")
    print(f"Content-Length: {request.content_length} bytes")
    if request.content_length and request.content_length > 0:
        print(f"Raw data (hex): {request.get_data().hex()}")


@app.errorhandler(413)
def request_entity_too_large(error):
    print(f"413 Error: Request size exceeded limit. Received: {request.content_length} bytes")
    return jsonify({"error": "Request entity too large", "size": request.content_length}), 413


@app.route("/register/<uid>", methods=["POST"])
def register(uid):
    """
    Register a user <uid> with an Ed25519 OpenSSH public key.
    Requirements:
      - <uid> must be in the ALLOWED_UIDS set.
      - <uid> must not already be registered.
      - The provided key must be Ed25519 format in OpenSSH encoding.
    """
    if uid not in ALLOWED_UIDS:
        actions_log.append({
            "args": ["register", uid],
            "status": 403,
            "error": "Unauthorized UID"
        })
        return "Error: UID is not in the allowed set.\n", 403

    if uid in user_keys:
        actions_log.append({
            "args": ["register", uid],
            "status": 403,
            "error": "UID already registered"
        })
        return "Error: User has already been registered.\n", 403

    pubkey_raw = request.get_data()
    try:
        public_key = serialization.load_ssh_public_key(
            pubkey_raw, backend=default_backend()
        )
    except Exception as e:
        actions_log.append({
            "args": ["register", uid],
            "status": 403,
            "error": "Error parsing the SSH public key"
        })
        return f"Error parsing the SSH public key: {str(e)}.\n", 403

    if not isinstance(public_key, ed25519.Ed25519PublicKey):
        actions_log.append({
            "args": ["register", uid],
            "status": 403,
            "error": "Non-Ed25519 public key"
        })
        return "Error: The key is not an ed25519 public key.\n", 403

    user_keys[uid] = public_key
    actions_log.append({
        "args": ["register", uid],
        "status": 200,
        "message": "Registration successful"
    })
    return f"Registration for {uid} successful.\n", 200


@app.route("/login/<uid>", methods=["POST"])
def login(uid):
    """
    Login a user <uid> by verifying a signature generated with the user's private key.
    Requirements:
      - <uid> must be in ALLOWED_UIDS.
      - <uid> must be already registered.
      - Provided data must be a valid signature for the message b'Authenticate'
        under the user's stored Ed25519 public key.
    """
    if uid not in ALLOWED_UIDS:
        actions_log.append({
            "args": ["login", uid],
            "status": 403,
            "error": "Unauthorized UID"
        })
        return "Error: UID is not in the allowed set.\n", 403
    if uid not in user_keys:
        actions_log.append({
            "args": ["login", uid],
            "status": 404,
            "error": "User not found"
        })
        return "Error: User is not registered.\n", 404

    signature = request.get_data()
    public_key = user_keys[uid]
    message = b"Authenticate"
    try:
        public_key.verify(signature, message)
    except Exception as e:
        actions_log.append({
            "args": ["login", uid],
            "status": 403,
            "error": "Signature verification failed"
        })
        return "Error: Signature verification failed.\n", 403

    actions_log.append({
        "args": ["login", uid],
        "status": 200,
        "message": "Login successful"
    })
    return f"Login for {uid} successful.\n", 200


@app.route("/peek", methods=["GET"])
def peek():
    return jsonify(actions_log), 200


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, debug=True)
