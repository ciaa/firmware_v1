#!/bin/sh

# A) make sure the current path is where this script lies.
cd `dirname $0`

# B) remove old DOC
. ./cleanup_doxygen_documentation.sh
cd `dirname $0`

# C) copy images to api/all
mkdir -p html/all/images
mkdir -p html/api/images

cp graphics/*.png graphics/*.gif html/all/images
cp graphics/*.png graphics/*.gif html/api/images

# D) create Doc
doxygen DoxyfileAll
doxygen DoxyfileApi
