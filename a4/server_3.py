from flask import Flask, request, jsonify
from cryptography.hazmat.primitives.asymmetric import ed25519
from cryptography.hazmat.primitives import serialization
from datetime import datetime, timedelta
import base64

app = Flask(__name__)
app.config["MAX_CONTENT_LENGTH"] = 1024
SECRET_KEY = "supersecret"
user_keys = {"test1":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAINa28FkCEVlkeak57tSmdp680clF4gDouNTZzh21rDU0 kakashi@assignment4", 
             "test2":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIJWXYJsSE2Nn84dE+ns1v7A5YFD2Eg35FLbfrl8b6xzF kakashi@assignment4",
             "test3":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAICftfCVCrZthekJm8rFFlfCsrd+LE0er2TQh0QiOXL5U kakashi@assignment4",
             "test4":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIMSlBMk6C0Nn71g4J/oDtYL8VECDIR3mIZrIDYrD+rpU kakashi@assignment4",
             "test5":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIFpOHL7UnwJxhE4k5GwNRdUF+W9IWkhMNpAq6UmvMYNw kakashi@assignment4"
             }
allowed_uids = {"test1", "test2", "test3", "test4", "test5"}

# Lets assume here that the registration was done properly. Since we have all the users registered, the implementation has been removed for brevity.

@app.route("/login/<uid>", methods=["POST"])
def login(uid):
    # Assume checks for user being in allowed list and whether registered are checked properly.
    public_key_str = user_keys[uid]
    try:
        public_key = serialization.load_ssh_public_key(public_key_str.encode("utf-8"))
        if not isinstance(public_key, ed25519.Ed25519PublicKey):
            raise ValueError("Only Ed25519 keys are allowed")
        signature = base64.b64decode(request.data.decode("utf-8").strip())   
        message = datetime.now().strftime("%Y-%m").encode("utf-8") 
        public_key.verify(signature, message)
        return jsonify({"message": "Authentication successful"})
    except Exception as e:
        return jsonify({"error": "Authentication failed", "details": str(e)}), 401

if __name__ == "__main__":
    app.run(debug=True, port=8003)