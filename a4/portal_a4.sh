#!/bin/bash -e

# Welcome to The Assignment Portal of CS 453/698 - Software and Systems Security
#
# For this assignment, we are not using the ugster system. We will be playing around with
# servers, such as creating them, accessing them and attacking them simply via python scripts.
#
# The file you are currently viewing is Bash-based client to assist you with 
# the building and attack modules required to complet your assignment. 
# This script is designed for a UNIX-like system (e.g., Ubuntu,
# MacOS, or WSL on Windows) with `bash`, `curl`, and `ssh-keygen` available.
#
# To use this file,
# - save it as a script (e.g., `portal_a4.sh`)
# - change its permission if needed (e.g., `chmod +x portal_a4.sh`) and
# - check its help message `./portal_a4.sh help`

# course variables (SET BY USERS)
USR_DEFAULT=""
KEY_DEFAULT=""

# course variables (SET BY ADMIN) 
GATEWAY=http://localhost:8000
NAMESPACE=assignment4

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
This is a Bash-based Client for the Assignment 4 of CS 453/698

Before using this script, please check that you have set the course variables
correctly on top of this file, including:
- USR_DEFAULT, your UWaterloo username (currently set to "${USR_DEFAULT}")
- KEY_DEFAULT, a path prefix for your key files (currently set to "${KEY_DEFAULT}")

Alternatively, you can supply the same information via environment variables
before calling this script, e.g.,
U=<username> K=<path-prefix-to-passkey> ./portal.sh <command>

Your current username is "${USR}" and path-prefix to passkey is "${KEY}".

The following commands are available for you to use:
- help: print this help message
- register <test[1-5]>: register a new user <testN> on your own passkey server
- login <test[1-5]>: login as a new user <testN> on your own passkey server
- peek : examine the registration and login records of a server
- attack <server-name>: launch the attack using your own script against the provided servers
EOF
}

# command (special): generate a key pair and register a user with the public key
function cmd_register() {
  ssh-keygen -t ed25519 -C "${USR}@${NAMESPACE}" -f "${KEY}_$2" -P ""
  curl -s "${GATEWAY}/$1/$2" -d @"${KEY}_$2.pub"
  echo ""
  echo "!!! IMPORTANT !!!"
  echo "- Please keep your private key in a safe place."
  echo "- If you lose it, you will LOSE ACCESS TO THE SYSTEM FOREVER."
}

# command (proxy): run a generic query via proxy
function cmd_login() {
  python3 generate_sig.py "${KEY}_$2"
  curl -s -X POST --data-binary @signature_"$2".bin "${GATEWAY}/$1/$2"
  
  # Check the response and provide feedback
  if [[ $? -eq 0 ]]; then
    echo "Request to server was successful."
  else
    echo "Error in sending request."
    exit 1
  fi

}

# command (special): bridge into another users' recent proxy requests
function cmd_peek() {
  curl "${GATEWAY}/$1"
}

function cmd_generate_userid_signature() {
  curl -s -X POST "${GATEWAY}/$1/$2"
}

function cmd_attack() {
  curl -s -X POST "${GATEWAY}/$1/$2"
}


# main entrypoint
if [ -n "$1" ]; then
  case "$1" in
  help)
    cmd_help
    ;;
  register)
    cmd_register "register" "$2"
    ;;
  peek)
    cmd_peek "peek"
    ;;
  attack)
    cmd_attack "attack" $2
    ;;
  login)
    cmd_login "login" "$2"
    ;;
  generate_userid_signature)
    cmd_generate_userid_signature "generate_userid_signature" "$2"
    ;;
  *)
    echo "unknown command, please see run '$0 help' for the help message"
    ;;
  esac
else
  cmd_help
fi
