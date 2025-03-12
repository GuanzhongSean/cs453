URL="http://ugster72d.student.cs.uwaterloo.ca/a3/"
SANDBOX1="sandbox1"
SANDBOX2="sandbox2"
SANDBOX3="sandbox3"
SANDBOX4="sandbox4"
FLAG="flag"
sudo apt-get update
yes | sudo DEBIAN_FRONTEND=noninteractive apt-get -yq install gcc gcc-multilib make
wget "${URL}${SANDBOX1}"
wget "${URL}${SANDBOX2}"
wget "${URL}${SANDBOX3}"
wget "${URL}${SANDBOX4}"
wget "${URL}${FLAG}"
chmod u+x sandbox*
