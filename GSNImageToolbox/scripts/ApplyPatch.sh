#/bin/bash
cd ../GSNImageToolbox/3pp/VisualMagick
git apply --reject --whitespace=fix ../../patches/ProjectFile.cpp.patch || true