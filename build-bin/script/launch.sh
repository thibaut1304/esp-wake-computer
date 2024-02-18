#!/bin/bash

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
# (Si vous avez d'autres bibliothèques à ajouter, faites-le ici)
echo "Installation des bibliothèques nécessaires..."
while read library; do
  ./arduino-cli lib install "$library"
done < required-libraries.txt

# Demander à l'utilisateur d'entrer ses informations pour le WiFi
read -p "Entrez le SSID: " ssid
read -s -p "Entrez le mot de passe: " password
echo

# Remplacer les placeholders dans le fichier source
sed -i "s/your_ssid/$ssid/" yourSketchName.ino
sed -i "s/your_password/$password/" yourSketchName.ino

# Compiler le sketch
echo "Compilation du sketch..."
./arduino-cli compile --fqbn esp32:esp32:esp32wrover yourSketchName.ino

echo "Terminé ! Votre binaire est prêt."

echo "Compilation du sketch..."
./arduino-cli compile --fqbn esp32:esp32:esp32wrover yourSketchName.ino

# Déplacer le binaire compilé vers le volume partagé
mv yourSketchName.ino.bin /workspace/bin/

echo "Terminé ! Votre binaire est dans le dossier /workspace/bin."
