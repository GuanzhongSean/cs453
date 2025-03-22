from flask import Flask, request, jsonify
from cryptography.hazmat.primitives.asymmetric import ed25519
from cryptography.hazmat.primitives import serialization

app = Flask(__name__)
app.config["MAX_CONTENT_LENGTH"] = 1024  # 1KB
SECRET_KEY = "supersecret"
user_keys = {"test1": "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIFW9mz4qwMzpQvR/tKtt7hLARNuGkYjfCBqrj/oksiFJ naruto@assignment4",
             "test2": "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAINdp4Fa3rpSGvT8P/kt6L5CoDaMsXj3FZfTf3DqApFaT naruto@assignment4",
             "test3": "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAINFJ9/u73UkZhe7RPSucM99MPnrfS1VVpfSvKrzAFEbd naruto@assignment4",
             "test4": "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIFNA17Coul03/KlnIYICKvPk0hwWSGdNEb2+/NT5LcSB naruto@assignment4",
             "test5": "ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAII6VDXGx2Zyug5p5V1u34IJ+lZWG94hYAuABgsod/IGj naruto@assignment4"
             }
allowed_uids = {"test1", "test2", "test3", "test4", "test5"}

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


@app.route("/login/<uid>", methods=["POST"])
def login(uid):
    print(f"Processing login for {uid}")
    if uid not in allowed_uids:
        actions_log.append({
            "args": ["login", uid],
            "status": 403,
            "error": "Unauthorized UID"
        })

    if uid not in user_keys:
        actions_log.append({
            "args": ["login", uid],
            "status": 404,
            "error": "User not found"
        })

    return jsonify({"message": "Authentication successful"})


@app.route("/peek", methods=["GET"])
def peek():
    print("Fetching actions log:")
    return jsonify(actions_log)


if __name__ == "__main__":
    app.run(debug=True, port=8001)
