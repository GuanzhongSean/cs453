# Part 1: About Authentication Schemes

## Q1

- **Function A** typically corresponds to how the client transforms or encodes the **public key** $v$. In many passkey setups, this might be some operation that turns the user’s public key into a format to be sent to the server (e.g., an OpenSSH-encoded string).
- **Function B** usually refers to how the client uses the **private key** $s$ to produce a verifiable value (often a **digital signature**). The input is the private key, and the output is the signature included in the login request.
- **Function C** is the server-side verification process. Given the stored public key and the signature, the server checks whether they match correctly (i.e., it verifies the signature against the public key). The output is a boolean: **true** if valid, **false** otherwise.

## Q2

- **Authenticity**: Because only the legitimate holder of the private key can generate a valid signature, a digital signature proves that the message came from the claimed user and not an imposter.
- **Integrity**: The signature covers the contents of the message, so if the message were altered, the signature verification would fail, detecting any tampering.

## Q3

- Slower hash functions like **bcrypt** include built-in “work factors” (or cost parameters) that make the hashing process more computationally expensive.
- This is beneficial for **password storage**, because even if an attacker obtains the password hashes, the added computational cost makes large-scale brute force or rainbow-table attacks far more time-consuming (and thus less feasible).
- In contrast, faster functions like **SHA-256** can be computed quickly and in parallel, which is great for many cryptographic operations but makes brute-force attacks easier when used for password hashing.
- This slows down a guessing attack significantly, but is barely noticed in the entire authentication protocol.

## Q4

1. **Preimage Resistance**: Given a hash value $y$, it's hard to find any $x$ such that $h(x) = y$.

   - **Broken implication**: Attackers could reverse-engineer passwords or other sensitive data from their hashes.

2. **Second Preimage Resistance**: Given $x_1$, it's hard to find a different $x_2$ such that $h(x_1) = h(x_2)$.

   - **Broken implication**: Attackers could forge data by finding alternate inputs that produce the same hash, undermining authenticity.

3. **Collision Resistance**: It should be infeasible to find _any_ two distinct values $x_1$ and $x_2$ for which $h(x_1) = h(x_2)$.
   - **Broken implication**: Attackers could generate pairs of malicious and benign messages that hash to the same value, enabling signature forgeries and other serious exploits.

---
