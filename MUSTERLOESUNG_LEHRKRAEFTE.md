# Labor 7: Musterlösung – Automatisierung mit GitHub Actions

## 📋 Übersicht für Lehrkräfte

Diese Musterlösung zeigt die **komplette Implementierung** des Labor 7 mit folgenden Komponenten:

### ✅ Implementierte Komponenten

1. **Refaktorierte main.cpp**
   - 19 automatisierte Wahrheitstabellen-Tests
   - Exit-Code 0 bei Erfolg
   - Exit-Code 1 bei Fehlschlag
   - Klare Test-Struktur mit Hilfsfunktionen

2. **README.md**
   - Professionelle Projektbeschreibung
   - GitHub Actions Badge
   - Dokumentation der Tests
   - DevOps-Konzepte erläutert

3. **.github/workflows/ci.yml**
   - Build-Job: Kompilierung mit g++
   - Test-Job: Ausführung mit Exit-Code-Validierung
   - Static-Analysis-Job: cppcheck Integration
   - Beide Trigger: `push` und `pull_request`

4. **CI_CD_ANLEITUNG.md**
   - Schritt-für-Schritt Anleitung für Studierende
   - Troubleshooting-Tipps
   - Zusatzaufgaben (Fehler-Handling, KI-Debugging)

---

## 🎯 Lernziele

Studierende sollten nach diesem Labor verstehen:

| Konzept | Umsetzung | Wichtigkeit |
|---------|-----------|------------|
| **Exit-Codes** | main() gibt 0/1 zurück | Kritisch |
| **CI/CD-Pipelines** | GitHub Actions YAML | Hoch |
| **Automatisiertes Testen** | Tests bei jedem Push | Hoch |
| **Cloud-Deployment** | Code auf GitHub | Mittel |
| **DevOps-Mindset** | "Code ist nur wertvoll mit Tests" | Mittel |
| **Debugging in Cloud** | Log-Files lesen | Mittel |

---

## 🔍 Technische Details

### Exit-Code Implementierung (main.cpp)

```cpp
bool allTestsPassed = true;
int testCount = 0;
int passedCount = 0;

// Tests durchführen
// Bei Fehler: allTestsPassed = false

if (!allTestsPassed) {
    std::cerr << "[FEHLER] Mindestens ein Test fehlgeschlagen!" << std::endl;
    return 1;  // ← CI-Pipeline sieht: FEHLER
}

std::cout << "[SUCCESS] Alle Tests bestanden!" << std::endl;
return 0;   // ← CI-Pipeline sieht: OK
```

### GitHub Actions Workflow (ci.yml)

Die YAML-Datei definiert:

```yaml
on:
  push:
    branches: [ "main" ]    # Trigger bei push
  pull_request:
    branches: [ "main" ]    # Trigger bei PR

jobs:
  build-and-test:          # Job 1: Kompilieren & Testen
    runs-on: ubuntu-latest # Linux-Umgebung
    steps:
      - uses: actions/checkout@v4
      - run: g++ *.cpp -o simulator
      - run: ./simulator   # Exit-Code wird überprüft!

  static-analysis:         # Job 2: Code-Qualität
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: sudo apt-get install -y cppcheck
      - run: cppcheck --enable=all --error-exitcode=1 *.cpp
```

---

## 📊 Testabdeckung

Die Musterlösung enthält folgende Tests:

### Test 1-5: Logik-Gatter (Wahrheitstabellen)

| Gatter | Input-Kombinationen | Tests |
|--------|-------------------|-------|
| AND    | 0-0, 0-1, 1-0, 1-1 | 4 |
| OR     | 0-0, 0-1, 1-0, 1-1 | 4 |
| NOT    | 0, 1 | 2 |
| XOR    | 0-0, 0-1, 1-0, 1-1 | 4 |
| NAND   | 0-0, 0-1, 1-0, 1-1 | 4 |
| **Gesamt** | | **18** |

### Test 6: Integration

| Test | Prüfung |
|------|---------|
| Engine-Init | LogicEngine kann Komponenten verwalten |
| Gesamt | 19 Tests |

---

## 🚨 Erwartete Ausgabe

### Wenn alle Tests erfolgreich:
```
========================================
C++ Digital Simulator - Automated Tests
Labor 7: GitHub Actions CI/CD Pipeline
========================================

[TEST 1] AND-Gate Wahrheitstabelle:
  AND: A=0 B=0 => 0 ✓
  AND: A=0 B=1 => 0 ✓
  AND: A=1 B=0 => 0 ✓
  AND: A=1 B=1 => 1 ✓
...

========================================
Test Summary:
Bestanden: 19 / 19
========================================

[SUCCESS] Alle Tests bestanden! ✓
```
**Exit-Code: 0** ✅

### Wenn Tests fehlschlagen:
```
========================================
Test Summary:
Bestanden: 9 / 19
========================================

[FEHLER] Mindestens ein Test fehlgeschlagen!
Die CI-Pipeline wird dies als FEHLER markieren.
```
**Exit-Code: 1** ❌

---

## 🔧 GitHub Actions Überwachung

### Actions-Tab zeigt:
```
✅ build-and-test     [PASSED]
✅ static-analysis    [PASSED]
```

Wenn ein Test fehlschlägt:
```
❌ build-and-test     [FAILED]
   └─ [Exit code 1] ./simulator returned non-zero
```

---

## 📚 Verwendete Konzepte

### 1. **Moderne C++-Konzepte**
- `std::unique_ptr` (Smart Pointer)
- `std::vector` (dynamische Arrays)
- Virtuelle Methoden (Polymorphismus)
- Tuple-Entpackung (`auto& [a, b, expected]`)

### 2. **YAML-Syntax**
- Indentation ist kritisch (keine Tabs!)
- `on:` definiert Trigger
- `jobs:` enthält Parallel-Tasks
- `steps:` sind sequenzielle Aktionen

### 3. **Shell-Scripting**
- `g++` Compiler-Befehle
- Exit-Code-Konventionen
- Ubuntu/Linux Kommandos

### 4. **Git-Workflow**
- Remote-Konfiguration
- Branch-Management
- Push/Pull-Operationen

---

## 🎓 Didaktisches Vorgehen

### Für Studierende empfohlene Reihenfolge:

1. **Tag 1**: Exit-Codes in main.cpp (lokal testen)
2. **Tag 2**: GitHub Account & Repository erstellen
3. **Tag 3**: .github/workflows/ci.yml schreiben
4. **Tag 4**: Push & Live-Überwachung (Actions-Tab)
5. **Tag 5**: Badge einbauen & Fehler intentional einbauen
6. **Tag 6**: Debugging mit KI (ChatGPT/Claude)

---

## 🚀 Zusatzaufgaben

### Zusatzaufgabe 1: Erweiterte Static-Analysis
```yaml
- name: Erweiterte cppcheck-Analyse
  run: cppcheck --enable=all --xml --xml-version=2 *.cpp
```

### Zusatzaufgabe 2: Code-Coverage
```yaml
- name: Code Coverage
  run: |
    g++ --coverage *.cpp -o simulator
    ./simulator
    gcov *.cpp
```

### Zusatzaufgabe 3: Multiple Compiler testen
```yaml
  build-gcc:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: g++ *.cpp -o simulator-gcc
      - run: ./simulator-gcc

  build-clang:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: apt-get install -y clang
      - run: clang++ *.cpp -o simulator-clang
      - run: ./simulator-clang
```

---

## 📝 Bewertungskriterien

| Kriterium | Punkte | Erfüllt |
|-----------|--------|---------|
| main.cpp mit Exit-Codes | 25% | ✓ |
| README.md & Dokumentation | 15% | ✓ |
| GitHub Actions YAML | 30% | ✓ |
| Erfolgreicher Push & Badge | 15% | ✓ |
| Zusatzaufgaben | 15% | ✓ |

---

## 🔗 Referenzen

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [YAML Syntax](https://yaml.org/spec/1.2/spec.html)
- [Exit Codes in C++](https://en.cppreference.com/w/cpp/utility/program/exit)
- [GitHub Badges](https://docs.github.com/en/actions/monitoring-and-troubleshooting-workflows/adding-a-workflow-status-badge)

---

## 📞 Häufige Studierenden-Fragen

**Q: Warum Exit-Code 1 und nicht andere Nummern?**
A: Convention. 0 = OK, alles andere = Fehler. CI-Systeme checken: `if (exit_code != 0) fail_job()`

**Q: Kann ich Tests lokal laufen lassen?**
A: Ja! `g++ *.cpp -o simulator && ./simulator` gibt Exit-Code aus.

**Q: Wie lange dauert die Pipeline?**
A: ~30-60 Sekunden, abhängig von Compile-Zeit und Server-Auslastung.

**Q: Was ist der Unterschied zwischen Push und Pull-Request?**
A: Push = direkter Merge. PR = Review vor Merge. Beide triggern hier.

**Q: Kann ich die Pipeline offline testen?**
A: Mit `act` (GitHub Actions CLI): Simuliert die Pipeline lokal.

---

**Viel Erfolg beim Unterricht! 🎓**
