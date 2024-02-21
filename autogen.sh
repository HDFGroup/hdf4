#!/bin/sh
#
# Copyright by The HDF Group.
# Copyright by the Board of Trustees of the University of Illinois.
# All rights reserved.
#
# This file is part of HDF.  The full HDF copyright notice, including
# terms governing use, modification, and redistribution, is contained in
# the COPYING file, which can be found at the root of the source code
# distribution tree, or in https://www.hdfgroup.org/licenses.
# If you do not have access to either file, you may request a copy from
# help@hdfgroup.org.
#

# A script to reconfigure autotools for HDF4
echo
echo "**************************"
echo "* HDF4 autogen.sh script *"
echo "**************************"
echo

# If paths to autotools are not specified, use whatever the system
# has installed as the default. We use 'command -v <tool>' to
# show exactly what's being used (shellcheck complains that 'which'
# is non-standard and deprecated).
if test -z "${HDF4_AUTOCONF}"; then
    HDF4_AUTOCONF="$(command -v autoconf)"
fi
if test -z "${HDF4_AUTOMAKE}"; then
    HDF4_AUTOMAKE="$(command -v automake)"
fi
if test -z "${HDF4_AUTOHEADER}"; then
    HDF4_AUTOHEADER="$(command -v autoheader)"
fi
if test -z "${HDF4_ACLOCAL}"; then
    HDF4_ACLOCAL="$(command -v aclocal)"
fi
if test -z "${HDF4_LIBTOOLIZE}"; then
    # check for glibtoolize (likely found on MacOS). If not found - check for libtoolize
    HDF4_LIBTOOLIZE="$(command -v glibtoolize)"
    if [ ! -f "$HDF4_LIBTOOLIZE" ] ; then
        HDF4_LIBTOOLIZE="$(command -v libtoolize)"
    fi
fi
if test -z "${HDF4_M4}"; then
    HDF4_M4="$(command -v m4)"
fi

# Make sure that these versions of the autotools are in the path
AUTOCONF_DIR=$(dirname "${HDF4_AUTOCONF}")
LIBTOOL_DIR=$(dirname "${HDF4_LIBTOOLIZE}")
M4_DIR=$(dirname "${HDF4_M4}")
PATH=${AUTOCONF_DIR}:${LIBTOOL_DIR}:${M4_DIR}:$PATH


echo

# Run autotools in order
#
# When available, we use the --force option to ensure all files are
# updated. This prevents the autotools from re-running to fix dependencies
# during the 'make' step, which can be a problem if environment variables
# were set on the command line during autogen invocation.

# Some versions of libtoolize will suggest that we add ACLOCAL_AMFLAGS
# = '-I m4'. This is already done in commence.am, which is included
# in Makefile.am. You can ignore this suggestion.

# LIBTOOLIZE
libtoolize_cmd="${HDF4_LIBTOOLIZE} --copy --force"
echo "${libtoolize_cmd}"
if [ "$verbose" = true ] ; then
    ${HDF4_LIBTOOLIZE} --version
fi
${libtoolize_cmd} || exit 1
echo
echo "NOTE: You can ignore the warning about adding -I m4."
echo "      We already do this in an included file."
echo

# ACLOCAL
if test -e "${LIBTOOL_DIR}/../share/aclocal" ; then
    aclocal_include="-I ${LIBTOOL_DIR}/../share/aclocal"
fi
aclocal_cmd="${HDF4_ACLOCAL} --force -I m4 ${aclocal_include}"
echo "${aclocal_cmd}"
if [ "$verbose" = true ] ; then
    ${HDF4_ACLOCAL} --version
fi
${aclocal_cmd} || exit 1
echo
echo "NOTE: You can ignore the warning about adding subdir-objects."
echo "      This currently causes failures on older systems. It will"
echo "      be added once we fix the problem."
echo

# AUTOHEADER
autoheader_cmd="${HDF4_AUTOHEADER} --force"
echo "${autoheader_cmd}"
if [ "$verbose" = true ] ; then
    ${HDF4_AUTOHEADER} --version
fi
${autoheader_cmd} || exit 1
echo

# AUTOMAKE
automake_cmd="${HDF4_AUTOMAKE} --copy --add-missing --force-missing"
echo "${automake_cmd}"
if [ "$verbose" = true ] ; then
    ${HDF4_AUTOMAKE} --version
fi
${automake_cmd} || exit 1
echo

# AUTOCONF
# The "obsolete" warnings category flags our Java macros as obsolete.
# Since there is no clear way to upgrade them (Java support in the Autotools
# is not great) and they work well enough for now, we suppress those warnings.
autoconf_cmd="${HDF4_AUTOCONF} --force --warnings=no-obsolete"
echo "${autoconf_cmd}"
if [ "$verbose" = true ] ; then
    ${HDF4_AUTOCONF} --version
fi
${autoconf_cmd} || exit 1
echo

echo "*** SUCCESS ***"

echo
exit 0

