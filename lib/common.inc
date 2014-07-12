#!/usr/bin/env bash
#
# common functions (error handling et cetera)
#

banner="openssl-compare >>"
function fail() {
        # generic fail function for bash scripts
        # arg: 1 - custom error message
        # arg: 2 - file
        # arg: 3 - line number
        # arg: 4 - exit status
        echo "${banner} ERROR: ${1}."
        [[ ${2+defined} && ${3+defined} && ${4+defined} ]] && \
        echo "${banner} file: ${2}, line number: ${3}, exit code: ${4}. exiting!"
        exit 1
} ; trap 'fail "caught signal"' HUP KILL QUIT

function bold() {
	local text=${1}
	echo "$(tput bold)${text}$(tput sgr0)"
};

# Autodetect MD5 checker, md5sum, md5, openssl dgst -md5
if command -v md5sum >/dev/null 2>&1 ; then # md5sum is used on Linux
        echo "Using $(command -v md5sum) to verify checksums" >&2
        function md5check() {
                echo $(md5sum ${1} | awk '{ print $1 }')
        }
elif command -v md5 >/dev/null 2>&1 ; then # md5 is used on OS X
        echo "Using $(command -v md5) to verify checksums" >&2
        function md5check() {
                echo $(md5 ${1} | awk '{ print $4 }')
        }
elif command -v openssl >/dev/null 2>&1 ; then # openssl is used if none of the above are found.
        echo "Using $(command -v openssl) dgst -md5 to verify checksums" >&2
        function md5check() {
                echo $(openssl dgst -md5 ${1} | awk '{ print $2 }')
        }
else
        fail "No utility to check MD5 found."
        exit 1
fi

