# Changelog

## [4.0.1] - 2025-03-24

### 🧠 Ajouté
- Cooldown de 2 min avant nouvelle action de l'esp32
- Verifie l'etat de l'interupteur avant de re activer le relais si deja on ne re active pas le relais

### 🛠️ Modifié

### 🧹 Supprimé

## [4.0.0] - 2025-03-24

### 🧠 Ajouté
- Gestion complète du watchdog matériel avec `esp_task_wdt` pour redémarrage automatique. (10s)
- Ajout de la fonction `debug_log()` centralisée pour publier les logs MQTT avec horodatage.
- Vérification automatique de la mémoire libre avec redémarrage en cas de dépassement de seuil.
- Endpoint OTA `/update` sécurisé avec mot de passe pour mise à jour via navigateur.
- Logs formatés avec `"[temps] message"` pour débogage clair via MQTT.

### 🛠️ Modifié
- Suppression des références inutilisées à `RCSwitch`.
- Amélioration de la gestion du Wi-Fi et de la reconnexion MQTT.
- Refacto complete et optimisation de lecture et appel.

### 🧹 Supprimé
- Suppression des références Linux et gestion de l’état `off`.
- Suppression des anciennes fonctions de test et variables obsolètes.

---

## [Anciennes versions]

*(Historique précédent non suivi dans ce fichier)*

