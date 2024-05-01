#!/bin/bash

cd /workspace
# Installer arduino-cli
echo "Installation de arduino-cli..."
wget https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_64bit.tar.gz
tar xf arduino-cli_latest_Linux_64bit.tar.gz
chmod +x arduino-cli
rm arduino-cli_latest_Linux_64bit.tar.gz

# Mise à jour de l'index des cartes
./arduino-cli core update-index

# Ajouter le support ESP32
echo "Ajout du support pour ESP32..."
./arduino-cli core install esp32:esp32

# Installer les bibliothèques requises

python3 /workspace/parse-libraries.py
tail -f > /dev/null

echo "Installation des bibliothèques nécessaires..."
while read library; do
  ./arduino-cli lib install "$library"
done < bin/required-libraries.txt

# Compiler le sketch
echo "Compilation du sketch..."
./arduino-cli compile --fqbn esp32:esp32:esp32wrover esp-wake-computer/esp-wake-computer.ino

echo "Terminé ! Votre binaire est prêt."

# Déplacer le binaire compilé vers le volume partagé
mv /workspace/esp-wake-computer/esp-wake-computer.ino.bin /workspace/bin/

echo "Terminé ! Votre binaire est dans le dossier /workspace/bin."
