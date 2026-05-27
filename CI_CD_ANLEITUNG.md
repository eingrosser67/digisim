# Labor 7: GitHub Actions CI/CD – Komplette Anleitung

## 🎯 Ziel dieser Anleitung
Du wirst deinen C++-Simulator mit automatisierter Kompilierung und Testausführung über GitHub Actions in der Cloud verbinden.

---

## Phase 1: GitHub Account & Repository-Setup

### 1.1 GitHub Account erstellen
1. Gehe auf [github.com](https://github.com)
2. Klick oben rechts auf "Sign up"
3. Registriere dich mit deinem Account

### 1.2 Repository auf GitHub erstellen
1. Nach Login: Oben rechts auf **"+"** → **"New repository"**
2. Gib folgende Werte ein:
   - **Repository name**: `DigitalSimulator` (oder dein Projektname)
   - **Description**: "C++ Logic Gate Simulator with GitHub Actions"
   - **Visibility**: Public (oder Private – für GitHub Actions beide OK)
   - **WICHTIG**: Setze KEINE Haken bei "Add a README file" oder ".gitignore"!
   - Klick "Create repository"

### 1.3 Notiere dir die URL
GitHub wird dir jetzt die exakte Befehlsfolge anzeigen. Sie sieht etwa so aus:
```bash
git remote add origin https://github.com/DEIN_NAME/DigitalSimulator.git
git branch -M main
git push -u origin main
```

---

## Phase 2: Lokaler Code mit Exit-Codes

### 2.1 Was sind Exit-Codes?
Ein Exit-Code ist eine Zahl (0-255), die ein Programm beim Beenden zurückgibt:
- **0** = Erfolg (OK)
- **1** = Fehler (Nicht OK)
- **Andere Werte** = Verschiedene Fehlertypen

GitHub Actions überprüft diese Codes automatisch!

### 2.2 main.cpp Refactoring
Deine `main.cpp` muss so strukturiert sein:

```cpp
bool allTestsPassed = true;

// ... hier laufen deine Tests ...
// Wenn ein Test fehlschlägt:
// allTestsPassed = false;

if (!allTestsPassed) {
    std::cerr << "[FEHLER] Mindestens ein Test fehlgeschlagen!" << std::endl;
    return 1;  // ← EXIT-CODE 1: FEHLER
}

std::cout << "[SUCCESS] Alle Tests bestanden!" << std::endl;
return 0;   // ← EXIT-CODE 0: ERFOLG
```

### 2.3 Lokal testen
```bash
g++ *.cpp -o simulator
./simulator
echo $?    # Zeigt Exit-Code (0 oder 1)
```

---

## Phase 3: GitHub Actions Workflow

### 3.1 Verzeichnisstruktur erstellen

```bash
mkdir -p .github/workflows
touch .github/workflows/ci.yml
```

### 3.2 ci.yml schreiben

**ACHTUNG: Leerzeichen sind kritisch! Keine Tabs!**

```yaml
name: C++ Automated Tests

on:
  push:
    branches: [ "main" ]
  pull_request:
    branches: [ "main" ]

jobs:
  build-and-test:
    runs-on: ubuntu-latest

    steps:
      - name: Lade Code auf den Runner
        uses: actions/checkout@v4

      - name: Kompiliere den Simulator
        run: g++ *.cpp -o simulator

      - name: Führe Wahrheitstabellen aus
        run: ./simulator

  static-analysis:
    runs-on: ubuntu-latest

    steps:
      - name: Lade Code auf den Runner
        uses: actions/checkout@v4

      - name: Installiere cppcheck
        run: sudo apt-get install -y cppcheck

      - name: Statische Code Analyse
        run: cppcheck --enable=all --error-exitcode=1 *.cpp
```

---

## Phase 4: Push zu GitHub

```bash
# Lokales Repo (falls noch nicht eingerichtet)
git init
git add .
git commit -m "Initial commit mit CI/CD Setup"

# Remote konfigurieren (Werte von GitHub kopieren!)
git remote add origin https://github.com/DEIN_NAME/DigitalSimulator.git
git branch -M main
git push -u origin main
```

---

## Phase 5: GitHub Actions Live überwachen

### 5.1 Actions-Tab öffnen
1. Gehe zu deinem GitHub-Repository
2. Klick oben auf den **"Actions"**-Tab
3. Du siehst deinen Workflow laufen (orange = läuft, grün = erfolgreich, rot = fehler)

### 5.2 Details ansehen
- Klick auf den laufenden Workflow
- Du siehst Live-Output vom Ubuntu-Server
- Warte, bis "build-and-test" und "static-analysis" abgeschlossen sind

### 5.3 Status-Badge einbauen

Wenn dein Workflow **grün** ist:
1. Klick oben rechts auf die **"..."** (3 Punkte)
2. Wähle **"Create status badge"**
3. Kopiere die Markdown:
   ```markdown
   [![C++ Automated Tests](https://github.com/DEIN_NAME/DigitalSimulator/actions/workflows/ci.yml/badge.svg)](https://github.com/DEIN_NAME/DigitalSimulator/actions/workflows/ci.yml)
   ```
4. Füge es oben in deine **README.md** ein:
   ```markdown
   # C++ Digital Simulator
   
   [![C++ Automated Tests](https://github.com/...)](...)
   
   Beschreibung...
   ```
5. Committen und pushen:
   ```bash
   git add README.md
   git commit -m "Füge Build-Badge hinzu"
   git push
   ```

---

## 🔧 Troubleshooting

### Problem: Workflow ist rot (fehlgeschlagen)
1. Klick auf den fehlgeschlagenen Workflow
2. Lese die Error-Message
3. Typische Fehler:
   - **`g++: command not found`** → Compiler nicht installiert
   - **`Permission denied`** → Fehlende execute-Berechtigung
   - **`Exit code 1`** → Dein Programm hat Tests fehlgeschlagen

### Problem: Exit-Code wird nicht erkannt
- Überprüfe, dass dein Programm `return 1;` am Ende hat
- Teste lokal: `./simulator && echo "Success" || echo "Failed"`

### Problem: cppcheck schlägt fehl
- Lese die Warnung genau
- Typische Probleme:
  - Ungenutzte Variablen
  - Speicherlecks
  - Fehlende Initialisierungen
- Behebe diese, committe und pushe erneut

---

## 🚀 Zusatzaufgabe 2: Absichtlicher Crash testen

### 2.1 Fehler einbauen
Ändere deine Gatter-Logik absichtlich (z.B. in AndGate.cpp):
```cpp
// FALSCH (für Test):
bool AndGate::update() {
    output = inputA | inputB;  // ← Sollte & sein!
    return output;
}
```

### 2.2 Pushen
```bash
git add AndGate.cpp
git commit -m "INTENTIONALER FEHLER: Teste Pipeline-Sicherheit"
git push
```

### 2.3 Pipeline-Fehler beobachten
- GitHub Actions wird ROT (Fehler)
- Klick auf "build-and-test" → Sieh den fehlgeschlagenen Test
- Kopiere die Fehlermeldung

### 2.4 KI-Debugging
Nutze ChatGPT/Claude mit:
```
Meine GitHub Actions Pipeline crasht mit diesem Output:
[Fehlermeldung einfügen]

Erklär mir, was passiert ist. Gib mir NICHT den reparierten Code, 
nur die Erklärung.
```

### 2.5 Selbst reparieren
- Behebe den Fehler lokal
- Teste: `g++ *.cpp -o simulator && ./simulator`
- Wenn grün: Committen und pushen
- Pipeline sollte grün werden ✓

---

## 📝 Checkliste zum Abhaken

- [ ] GitHub Account erstellt
- [ ] Repository auf GitHub angelegt
- [ ] Lokaler Code hat Exit-Codes
- [ ] `.github/workflows/ci.yml` erstellt
- [ ] Code zu GitHub gepusht
- [ ] Actions-Workflow ist grün
- [ ] README.md Badge eingebaut
- [ ] Absichtlicher Fehler zum Testen eingebaut und repariert

---

## 🎓 Was du gelernt hast

✅ **DevOps**: Automatisierte Pipelines für CI/CD  
✅ **Cloud**: Code in der Cloud kompilieren/testen  
✅ **Zuverlässigkeit**: Tests fangen Fehler automatisch  
✅ **Skalierbarkeit**: Dieser Workflow funktioniert mit 100 Testfällen genauso  
✅ **Industrie-Standard**: Das nutzen Google, Facebook, Netflix, ...  

---

**Herzlichen Glückwunsch – dein Projekt ist jetzt professionell! 🚀**
