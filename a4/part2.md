# Part 2: Building a passkey-based authentication scheme

## 1. Dependencies

```bash
pip install flask cryptography
```

**Dependencies**:

- **Flask** (for the HTTP server)
- **cryptography** (for parsing SSH keys and verifying Ed25519 signatures)

## 2. How to Run

```bash
python3 part2_server.py
```

## 3. Endpoints & Requirements

### 3.1 `POST /register/<uid>`

- **Allowed `uid`s**: `{"test1", "test2", "test3", "test4", "test5"}`.
- **Body**: Should contain the user’s **OpenSSH-encoded Ed25519 public key** (e.g., `ssh-ed25519 AAAAC3... user@host`).
- If the user is already registered or the key is invalid, the server responds with a 403/404 error. Otherwise, it responds with “Registration successful.”.

### 3.2 `POST /login/<uid>`

- **Allowed `uid`s**: same set as above.
- **Body**: Contains a **raw signature** (binary) created by signing the message `b"Authenticate"` with the user’s private key.
- The server verifies the signature using the stored Ed25519 public key. If it matches, returns 200; otherwise, returns an error code.

### 3.3 `GET /peek`

- **Debug only**: Returns a JSON array of the `actions_log`, listing each registration or login attempt with status codes and optional error messages.

## 4. Approach / Design Notes

1. **Storing User Keys**:
   We keep a simple in-memory dictionary named `user_keys` that maps `<uid>` to its `Ed25519PublicKey` object. This means that once the process restarts, all registrations are lost. This is sufficient for the scope of the assignment.

2. **Allowed UIDs**:
   We only allow `test1, test2, test3, test4, test5` as valid `<uid>`. Any other `<uid>` triggers a 403 response.

3. **Signature Verification**:
   For `POST /login/<uid>`, the server calls `public_key.verify(signature, b"Authenticate")`. If it raises an exception, we respond with a 403 indicating “Signature verification failed.”

4. **Testing**:
   - We use the **`portal_a4.sh`** script to issue `register <testN>` and `login <testN>` requests.
   - `register` generates a fresh Ed25519 key pair and sends the public key to `/register/...`.
   - `login` uses `generate_sig.py` to sign the message `Authenticate` with the corresponding private key, then sends that signature to `/login/...`.

---
