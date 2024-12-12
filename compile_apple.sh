## Needed for compile on an Arm Device:
# softwareupdate --install-rosetta
# /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# arch -x86_64 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# brew install sdl2 sdl2_mixer sdl2_image sdl2_ttf git qt make 
# arch -x86_64 /usr/local/bin/brew install sdl2 sdl2_mixer sdl2_image sdl2_ttf git qt make 

# Set environment paths for x86 architecture
export CPATH=/usr/local/include:$CPATH
export LIBRARY_PATH=/usr/local/lib:$LIBRARY_PATH

# Clean previous builds if necessary
rm -rf build

# Create build directory
mkdir build

# Enter build directory
cd build

# Build x86
echo "Building for x86_64..."
qmake QMAKE_APPLE_DEVICE_ARCHS=x86_64 ..
make
make clean
mv eZeus eZeus_x86

echo "x86_64 build completed successfully!"

# Build arm64
echo "Building for ARM64..."
qmake QMAKE_APPLE_DEVICE_ARCHS=arm64 ..
make
make clean
mv eZeus eZeus_arm64

rm Makefile

mkdir libs_x86 libs_arm64 libs

dylibbundler -of -cd -b -x "eZeus_x86" -d "libs_x86" -p "@executable_path/libs/"
dylibbundler -of -cd -b -x "eZeus_arm64" -d "libs_arm64" -p "@executable_path/libs/"

cd libs_arm64

for a in *.dylib; do lipo "$a" ../libs_x86/"$a" -output ../libs/"$a" -create; done

cd ..

rm -rf libs_*

lipo eZeus_arm64 eZeus_x86 -output eZeus -create

rm eZeus_*

echo "Builds completed!"
