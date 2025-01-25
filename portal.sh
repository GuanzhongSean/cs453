#!/bin/bash -e

# Welcome to The Assignment Portal of CS 489/698 - Software and Systems Security
#
# This is not a regular website for broadcasting information. Instead, it is a
# web service hidden behind the HTTP protocol and only intended for users of the
# Software and Systems Security course for assignment-related tasks.
#
# The file you are currently viewing is a Bash-based client to interact with
# the web service. This script is designed for a UNIX-like system (e.g., Ubuntu,
# MacOS, or WSL on Windows) with `bash`, `curl`, and `ssh-keygen` available.
#
# To use this file,
# - save it as a script (e.g., `portal.sh`)
# - change its permission if needed (e.g., `chmod +x portal.sh`) and
# - check its help message `./portal.sh help`
#
# Also check the FAQ section in the prompt.

# course variables (SET BY ADMIN)
host=ugster72d.student.cs.uwaterloo.ca
port=8000
GATEWAY=$host:$port
NAMESPACE=s3

# course variables (SET BY USERS)
USR_DEFAULT=j76xiao
KEY_DEFAULT=key

# alternative to set user info: environment variable
if [ -z "${U}" ]; then
  USR="${USR_DEFAULT}"
else
  USR="${U}"
fi

if [ -z "${K}" ]; then
  KEY="${KEY_DEFAULT}"
else
  KEY="${K}"
fi

# command: print help message
function cmd_help() {
  cat <<EOF
This is a Bash-based Client to the Assignment Portal of CS 489/698

Before using this script, please check that you have set the course variables
correctly on top of this file, including:
- USR_DEFAULT, your UWaterloo username (currently set to "${USR_DEFAULT}")
- KEY_DEFAULT, a path prefix for your key files (currently set to "${KEY_DEFAULT}")

Alternatively, you can supply the same information via environment variables
before calling this script, e.g.,
U=<username> K=<path-prefix-to-passkey> ./portal.sh <command>

Your current username is "${USR}" and path-prefix to passkey is "${KEY}".

The following commands are available for everyone for VM management
- help: print this help message
- register: create a ed25519 key pair and register the user with the public key
- status: check the status of the virtual machine allocated to the user
- launch: launch the virtual machine allocated to the user
- destroy: destroy the virtual machine allocated to the user
- ssh: SSH into the virtual machine allocated to the user

The following command are available if you are a system administrator
- reset <uid>: reset the user to a state before registration
- shutdown: send a shutdown request to the system

**Frequently Asked Questions (FAQs)**

Q: What is the path prefix for keys?
A: This is a location on your local filesystem where you store your key pairs.
   - The public key will be stored in a file with path: <path-prefix>.pub
   - The private key will be stored in a file with path: <path-prefix>

   For example,
   - If your path prefix is "my_path", your keys will be
     - "my_path.pub" and
     - "my_path"
   - If your path prefix is "/home/my_username/my_keys/course", you keys will be
     - "/home/my_username/my_keys/course.pub" and
     - "/home/my_username/my_keys/course"

Q: I received a warning about conflicting fingerprints when on the SSH trial
   into a newly re-created VM (i.e., destroyed and launched), is this normal?
A: Yes, this is expected and is a defense mechanism against man-in-the-middle
   (MiTM) attacks. To resolve this issue, simply follow the prompt and remove
   the offending fingerprint.

EOF
}

# command (special): generate a key pair and register a user with the public key
function cmd_special_register() {
  ssh-keygen -t ed25519 -C "${USR}@${NAMESPACE}" -f "${KEY}" -P ""
  echo ""
  echo "!!! IMPORTANT !!!"
  echo "- Please keep your private key in a safe place."
  echo "- If you lose it, you will LOSE ACCESS TO THE SYSTEM FOREVER."
  echo "!!! IMPORTANT !!!"
  echo ""
  curl -w '\n' -s "${GATEWAY}/${USR}/register" -d @"${KEY}.pub"
}

# command (special): ssh into the allocated VM
function cmd_special_ssh() {
  local C=$(cmd_generic "config")
  local X=(${C//:/ })
  ssh -i ${KEY} vagrant@${X[0]} -p ${X[1]}
}

# command (generic): run a generic query
function cmd_generic() {
  local T=$(date +%s | xargs echo -n)
  local S=$(echo -n "${T}" | ssh-keygen -Y sign -f "${KEY}" -n "${NAMESPACE}" -)
  curl -w '\n' -s -X POST "${GATEWAY}/${USR}/$1" -d "${S}"
}

# command (generic): run a generic query (for admin commands)
function cmd_generic_admin() {
  local T=$(date +%s | xargs echo -n)
  local S=$(echo -n "${T}" | ssh-keygen -Y sign -f "${KEY}" -n "${NAMESPACE}" -)
  curl -w '\n' -s -X POST "${GATEWAY}/${USR}/$1/$2" -d "${S}"
}

# command (generic): run a system action (for admin commands)
function cmd_generic_system() {
  local T=$(date +%s | xargs echo -n)
  local S=$(echo -n "${T}" | ssh-keygen -Y sign -f "${KEY}" -n "${NAMESPACE}" -)
  curl -w '\n' -s -X POST "${GATEWAY}/${USR}/$1" -d "${S}"
}

# main entrypoint
if [ -n "$1" ]; then
  case "$1" in
  help)
    cmd_help
    ;;
  register)
    cmd_special_register
    ;;
  status)
    cmd_generic "status"
    ;;
  launch)
    cmd_generic "launch"
    ;;
  destroy)
    cmd_generic "destroy"
    ;;
  ssh)
    cmd_special_ssh
    ;;
  reset)
    cmd_generic_admin "reset" "$2"
    ;;
  list)
    cmd_generic_system "list"
    ;;
  shutdown)
    cmd_generic_system "shutdown"
    ;;
  *)
    echo "unknown command, please see run '$0 help' for the help message"
    ;;
  esac
else
  cmd_help
fi
