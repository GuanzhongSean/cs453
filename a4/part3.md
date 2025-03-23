# Part 3: Identify exploits for vulnerable authentication schemes

## `server_1`

`server_1` is vulnerable because it never verifies user credentials or signatures during login. Anyone—even without a valid private key—can "log in" successfully. Even if the user is not in `allowed_uids` or not in `user_keys`, the code only appends an entry to `actions_log`: it doesn’t return an error response. The function continues to the end and returns a success message anyway. Attackers can trivially impersonate any registered user. Since the server never verifies a signature, you don’t need the private key or a valid signature at all. Simply POST _anything_ to `/login/<uid>` and you’ll get `"Authentication successful"`.

## `server_2`

`server_2` is vulnerable because its logic lets you bypass cryptographic verification altogether if the signature exactly matches the stored public key string. Specifically:

```python
parts = public_key_str.split(";")
expected = parts[1] if len(parts) > 1 else public_key_str
if signature != expected:
    # Do real cryptographic verification here
    public_key = serialization.load_ssh_public_key(public_key_str.encode("utf-8"))
    signature = request.data
    public_key.verify(signature)
```

- If you send any signature that is identical to `public_key_str` (the entire SSH public key string), the `if signature != expected:` check fails, and the code **skips** calling `public_key.verify(...)`.
- In other words, you can just **submit the public key itself** as your "signature". The server then never performs a cryptographic check and immediately returns `"Authentication successful"`.

Hence an attacker can trivially impersonate a user by providing that user’s stored public key text as the signature, bypassing any real signature check. Thus, **server_2** is indeed vulnerable.

## `server_3`

`server_3` is also vulnerable because it uses a fixed month-year string as the message to be signed, allowing replay attacks for the entire month. In the code, the server does:

```python
message = datetime.now().strftime("%Y-%m").encode("utf-8")
public_key.verify(signature, message)
```

The message is just the current year and month (for example, `b"2025-03"`). That means all legitimate logins for that month sign exactly the same message—no per-request uniqueness. If an attacker observes (or intercepts) a legitimate signature **once** during a given month, they can **reuse** that exact signature (base64-encoded) for the rest of the month, because the server never changes the message string within that month. It has no way to distinguish a reused signature from a fresh one.

---

## Example Attack Scenario

1. **Attacker** eavesdrops on the log and sees a base64-encoded signature from a legitimate user logging in on one day of March, which verifies `b"2025-03"`.
2. For the entire month of March, the attacker simply **reuses** that signature on any subsequent login requests to `/login/<uid>`.
3. Since the server still uses `b"2025-03"` as the message, it happily verifies the old signature and grants access, even though the signature was created by someone else days or weeks earlier.

Hence, `server_3` is vulnerable to replay attacks due to relying on a **stale, month-level “challenge”**. A properly designed system would require a **unique** challenge per login attempt—something that can’t be reused.

---
