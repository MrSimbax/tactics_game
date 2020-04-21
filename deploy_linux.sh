#!/bin/sh

# Assuming the build is already done
cd build

# Remove old deployed files if they exist
rm -f Tactics_Game*.AppImage
rm -rf tactics_game.AppDir/

# Download linuxdeploy tool if needed
wget -nc https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-x86_64.AppImage

# Create AppDir
./linuxdeploy-x86_64.AppImage -e tactics_game -d ../desktop/tactics_game.desktop -i ../desktop/dice3.png --appdir tactics_game.AppDir

# Add the assets/ folder
# TODO: create the Linux directory structure in make install and hardcode new paths
mkdir tactics_game.AppDir/
cp -r assets/ tactics_game.AppDir/usr/bin/assets

# Create AppImage
wget -nc https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x appimagetool-x86_64.AppImage
