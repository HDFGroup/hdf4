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

echo
echo "NOTE: You can ignore the warning about adding -I m4."
echo "      We already do this in an included file."
echo

# The "obsolete" warnings category flags our Java macros as obsolete.
# Since there is no clear way to upgrade them (Java support in the Autotools
# is not great) and they work well enough for now, we suppress those warnings.
autoreconf -Wno-obsolete --force --install
