## Needed for compile on an Arm Device:
# softwareupdate --install-rosetta
# /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# arch -x86_64 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
# brew install sdl2 sdl2_mixer sdl2_image sdl2_ttf git qt make 
# arch -x86_64 /usr/local/bin/brew install sdl2 sdl2_mixer sdl2_image sdl2_ttf git qt make 

export CPATH=/opt/homebrew/include:$CPATH
export LIBRARY_PATH=/opt/homebrew/lib:$LIBRARY_PATH

mkdir build_x86 arm64

cd build_x86

qmake ..

make -C ..
