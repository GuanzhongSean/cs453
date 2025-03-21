from flask import Flask, request, jsonify
from cryptography.hazmat.primitives.asymmetric import ed25519
from cryptography.hazmat.primitives import serialization

app = Flask(__name__)
app.config["MAX_CONTENT_LENGTH"] = 1024
SECRET_KEY = "supersecret"
user_keys = {"test1":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIE7XpKeJZF/nkzvKEfGouFbyRdq0B5RvVQAuWJl4JrG5 sasuke@assignment4", 
             "test2":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIMQFGt4HUIq4eeZekYCOdnZkfeIbvrUEKJAbca6F0k+5 sasuke@assignment4",
             "test3":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIIfCj+pgrot4KMR23Kz1LHhSRR6fRGhRJXTG9DtIE8yv sasuke@assignment4",
             "test4":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAICB0s4qkLRujfApYQlNbHSYiN39gneNju9lse5MevdsA sasuke@assignment4",
             "test5":"ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIIilONIeUNHTNsn2I5HuEftkhAzQ7IdIg9ZWcpDjkq3v sasuke@assignment4"
             }
allowed_uids = {"test1", "test2", "test3", "test4", "test5"}


@app.route("/login/<uid>", methods=["POST"])
def login(uid):
    public_key_str = user_keys[uid]
    signature = request.data.decode("utf-8").strip()
    try:
        parts = public_key_str.split(";")
        expected = parts[1] if len(parts) > 1 else public_key_str
        if signature != expected:
            public_key = serialization.load_ssh_public_key(public_key_str.encode("utf-8"))
            signature = request.data
            public_key.verify(signature)
        return jsonify({"message": "Authentication successful"})
    except Exception as e:
        return jsonify({"error": "Authentication failed", "details": str(e)}), 401

if __name__ == "__main__":
    app.run(debug=True, port=8002)