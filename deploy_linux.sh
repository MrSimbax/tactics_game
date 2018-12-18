#!/bin/sh

# Assuming the build is already done
cd build

# Remove old deployed files if they exist
rm -f Tactics_Game-x86_64.AppImage tactics_game.zip
rm -rf tactics_game.AppDir/

# Download linuxdeploy tool if needed
wget -nc https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-x86_64.AppImage

# Create AppImage
./linuxdeploy-x86_64.AppImage -e tactics_game -d ../desktop/tactics_game.desktop -i ../desktop/dice3.png --appdir tactics_game.AppDir --output appimage

# Zip
mv tactics_game tactics_game_bin
mkdir tactics_game
mv Tactics_Game-x86_64.AppImage tactics_game/Tactics_Game-x86_64.AppImage
cp -r assets/ tactics_game/assets
zip -9 tactics_game.zip -r tactics_game/assets tactics_game/Tactics_Game-x86_64.AppImage
rm -rf tactics_game
mv tactics_game_bin tactics_game

