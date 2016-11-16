#!/usr/bin/env bash

## ---------------------------------------------------------
## Install C++ libs and build the 'pmt' project.
## ---------------------------------------------------------

### Utils
function join_by { local IFS="$1"; shift; echo "$*"; }

echo "---------------------------------"
echo "### 1. Checking dependencies..."
echo "---------------------------------"

NEEDED_PACKAGES=(cmake)
PACKAGES_NOT_INSTALLED=()

for package in "${NEEDED_PACKAGES[@]}"
do
	echo -ne "#### checking '${package}'..."

	if ! dpkg-query -l "${package}" &> /dev/null; then
		echo "not installed!"
		PACKAGES_NOT_INSTALLED+=(${package})
	else
		echo "installed!"
	fi
done

if [ ${#PACKAGES_NOT_INSTALLED[@]} -gt 0 ]; then
	echo "Please install the following packages before building this software:"
	echo "sudo apt-get install `join_by ' ' ${PACKAGES_NOT_INSTALLED[@]}`"
	exit 1
fi

echo "---------------------------------"
echo "### 2. Downloading libraries...  "
echo "---------------------------------"


echo "---------------------------------"
echo "### 3. Running CMake...          "
echo "---------------------------------"

mkdir -p build
cd build
cmake ..

echo "---------------------------------"
echo "### 4. Running Makefile...       "
echo "---------------------------------"

make clean all

echo "All done!"
