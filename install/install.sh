#!/bin/bash
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ -f /etc/redhat-release ]; then
    # Red hat based
    echo "Not yet implemented!"
fi

if [ -f /etc/lsb-release ]; then
    # Debian based distributions
    echo "Installing dependencies for debian based distributions"
    sh ./debian/install_apt_packages.sh
    sh ./debian/install_python.sh
    sh ./debian/install_sfml.sh
    sh ./debian/install_rapidjson.sh
fi
