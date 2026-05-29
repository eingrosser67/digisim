# C++ Digital Simulator 🔌

[![C++ Automated Tests](https://github.com/dantschi/digisim/actions/workflows/ci.yml/badge.svg)](https://github.com/dantschi/digisim/actions/workflows/ci.yml)

Ein **C++-basierter Simulator für digitale Logikgatter** mit automatisierter CI/CD-Pipeline – ideal zum Verständnis von Schaltlogik, Speichermanagement und DevOps-Konzepten.

## 📋 Überblick

Dieses Projekt implementiert einen vollständigen Logik-Simulator mit verschiedenen digitalen Gattern:

- **AND-Gate** – Logisches UND
- **OR-Gate** – Logisches ODER  
- **NOT-Gate** – Logische Negation
- **XOR-Gate** – Exklusives ODER
- **NAND-Gate** – Negiertes UND

## Labor 10: Sequenzielle Logik, Takt & das D-Flip-Flop

### Phase 1: Die Klasse DFlipFlop

**Dateien**: `DFlipFlop.h` / `DFlipFlop.cpp`

Das D-Flip-Flop ist das elementarste Speicherbauteil:
- **1 Eingangs-Pin** (m_inputs.size() == 1)
- **Private Variable** `bool m_storedState = false;` (der "Tresor")
- **evaluate()** gibt NICHT den Input aus, sondern `m_storedState` (Firewall!)
- **onClockTick()** speichert den Input-Wert bei der Taktflanke

**Kritisch**: Die Firewall-Funktion in `evaluate()`:
```cpp
void DFlipFlop::evaluate() {
    // NICHT: m_inputs[0]->evaluate();  ← Keine Rekursion!
    m_output = m_storedState;           // ← Gibt alten Wert zurück
    m_isCalculated = true;
}
```

### Phase 2: Die 2-Phasen-Schleife & DFS-Korrektur

Die Simulation in **main.cpp** implementiert ein 4-Stufen-Zyklus pro Takt:

```
Phase A (Amnesie):
  Alle Gatter reset() aufrufen → m_isCalculated = false

Phase B (Lesen):
  Kombinatorische Gatter evaluieren
  NOT->evaluate() wird aufgerufen

Phase C (Ausgabe):
  DFF-Ausgang drucken

Phase D (Schreiben):
  onClockTick() auf alle Flip-Flops aufrufen
  Sie speichern die stabilen Werte
```

**Wichtig**: `notGate->evaluate()` MUSS in Phase B aufgerufen werden! Das DFF evaluiert seinen Input nicht (Firewall), daher muss die Logik davor explizit berechnet werden.

### Phase 3: Der 1-Bit Zähler

**Schaltung**: Ringverkabelung ohne Stack Overflow!
```
DFlipFlop --[NOT]--+
^                  |
|__________________|
```

**Verhalten**: Die Ausgabe toggelt bei jedem Takt
- Takt 0: Output = 0
- Takt 1: Output = 1
- Takt 2: Output = 0
- Takt 3: Output = 1
- ...

**Warum funktioniert das?**
1. Die Firewall (DFF.evaluate()) bricht die Rekursion ab
2. Die 2-Phasen-Trennung sorgt für stabile Werte
3. onClockTick() speichert erst nach vollständiger Berechnung

### 🎯 Zusatzaufgabe 1: D-Flip-Flop mit Enable-Pin (DFFE)

**Dateien**: `DFlipFlopEnable.h` / `DFlipFlopEnable.cpp`

```cpp
// 2 Pins statt 1:
//   Pin 0: D (Daten-Eingang)
//   Pin 1: EN (Enable-Signal)

void DFlipFlopEnable::onClockTick() {
    bool enableSignal = m_inputs[1]->getOutput();
    bool dataValue = m_inputs[0]->getOutput();
    
    if (enableSignal) {
        m_storedState = dataValue;  // Speichere NUR wenn EN=1
    }
}
```

**Anwendung**: Zähler können pausiert/angehalten werden

### 🎯 Zusatzaufgabe 2: Der 2-Bit Zähler

**Datei**: `main_zusatz.cpp`

**Schaltung**:
```
Bit 0 (Standard 1-Bit Ring):
  DFF0 ←→ NOT0

Bit 1 (Kaskadierter Zähler):
  XOR1-Input0 = DFF0-Output (Bit 0)
  XOR1-Input1 = DFF1-Output (Bit 1)
  DFF1 ← XOR1-Output
```

**Zähler-Sequenz**: 0 (00) → 1 (01) → 2 (10) → 3 (11) → 0 (00) ...

**Wie es funktioniert**:
- **Bit 0** toggelt JEDEN Takt: 0 → 1 → 0 → 1 ...
- **Bit 1** toggelt NUR wenn Bit 0 = 1 ist:
  - XOR(0, x) = x (kein Toggle)
  - XOR(1, x) = ¬x (Toggle)

Dies ist die korrekte Carry-Logik für binäre Zähler!

## 🔧 Compilation

```bash
# 1-Bit Zähler (Phase 1-3)
g++ -std=c++17 -o simulator main.cpp Component.cpp NotGate.cpp AndGate.cpp \
    OrGate.cpp XorGate.cpp NandGate.cpp Switch.cpp DFlipFlop.cpp

# Mit Zusatzaufgaben (DFFE + 2-Bit Zähler)
g++ -std=c++17 -o simulator_zusatz main_zusatz.cpp Component.cpp NotGate.cpp \
    AndGate.cpp OrGate.cpp XorGate.cpp NandGate.cpp Switch.cpp DFlipFlop.cpp \
    DFlipFlopEnable.cpp
```

## ▶️ Ausführung

```bash
# 1-Bit Zähler (10 Takte)
.\simulator.exe

# Zusatzaufgaben Demo
.\simulator_zusatz.exe
```

## ✅ Fehler-Checkliste

| Problem | Ursache | Lösung |
|---------|---------|--------|
| Stack Overflow | DFF ruft Input->evaluate() auf | DFF->evaluate() darf Input nicht evaluieren |
| Output bleibt 0 | NOT wird nicht evaluiert | `notGate->evaluate()` in Phase B aufrufen |
| Kompilierungsfehler | onClockTick() auf Gate-Pointer | Verwende separate `std::vector<std::shared_ptr<DFlipFlop>>` |
| Wert ändert sich nie | onClockTick() wird nicht aufgerufen | Phase D-Schleife über allFlipFlops überprüfen |

## 📚 Lernziele

✅ D-Flip-Flops als Firewall gegen Zyklen verstehen
✅ 2-Phasen-Simulation (Lesen/Schreiben) implementieren
✅ Post-Order DFS mit Memoization anwenden
✅ Ringverkabelung ohne Endlosschleife
✅ Kaskadierte/Multi-Bit-Zähler bauen
✅ Enable-Signale für selektive Speicherung nutzen

## 📂 Labor 10 - Dateien

| Datei | Inhalt |
|-------|--------|
| `DFlipFlop.h` | Basis D-Flip-Flop Header |
| `DFlipFlop.cpp` | Basis D-Flip-Flop Implementierung |
| `DFlipFlopEnable.h` | DFFE Header (Zusatzaufgabe 1) |
| `DFlipFlopEnable.cpp` | DFFE Implementierung (Zusatzaufgabe 1) |
| `main.cpp` | 1-Bit Zähler mit 2-Phasen-Simulation |
| `main_zusatz.cpp` | DFFE + 2-Bit Zähler Demo (Zusatzaufgaben) |

---

```
├── Component.h/cpp        # Abstrakte Basisklasse für alle Gatter
├── LogicEngine.h/cpp      # Kern-Engine zur Verwaltung von Schaltungen
├── AndGate.h/cpp          # AND-Gatter-Implementierung
├── OrGate.h/cpp           # OR-Gatter-Implementierung
├── NotGate.h/cpp          # NOT-Gatter-Implementierung
├── XorGate.h/cpp          # XOR-Gatter-Implementierung
├── NandGate.h/cpp         # NAND-Gatter-Implementierung
├── main.cpp               # Automatisierte Test-Suite mit Exit-Codes
├── README.md              # Diese Datei
└── .github/
    └── workflows/
        └── ci.yml         # GitHub Actions CI/CD Pipeline
```

## 🎯 Kernkonzepte

### Polymorphismus
Alle Gatter erben von der abstrakten Basisklasse `Component` und implementieren die virtuelle Methode `update()`. Die `LogicEngine` verwaltet die Komponenten polymorphisch über `std::unique_ptr<Component>`.

### Speichermanagement
Das Projekt demonstriert modernes C++ Speichermanagement mit:
- **Smart Pointers** (`std::unique_ptr`) statt rohe Zeiger
- Automatische Speicherfreigabe ohne Memory Leaks
- RAII-Prinzip (Resource Acquisition Is Initialization)

### CI/CD & Automatisierung
GitHub Actions triggert automatisch bei jedem `git push`:
- ✅ Code-Kompilierung
- ✅ Wahrheitstabellen-Tests (19 automatisierte Tests)
- ✅ Exit-Code Validierung
- ✅ Statische Code-Analyse (cppcheck)

## 🚀 Kompilierung & Ausführung

### Voraussetzungen
- C++17 oder höher
- Ein C++-Compiler (GCC, Clang, MSVC)

### Lokal kompilieren
```bash
g++ *.cpp -o simulator
./simulator
```

### Erfolg (Exit-Code 0)
```
Test Summary:
Bestanden: 19 / 19
========================================

[SUCCESS] Alle Tests bestanden! ✓
```

### Fehler (Exit-Code 1)
```
[FEHLER] Mindestens ein Test fehlgeschlagen!
Die CI-Pipeline wird dies als FEHLER markieren.
```

## 🤖 Automatisierte Tests

Die `main.cpp` führt umfassende Wahrheitstabellen-Tests durch:

| Gatter | Tests | Status |
|--------|-------|--------|
| AND    | 4     | ✓      |
| OR     | 4     | ✓      |
| NOT    | 2     | ✓      |
| XOR    | 4     | ✓      |
| NAND   | 4     | ✓      |
| Integration | 1 | ✓      |
| **Gesamt** | **19** | **✓** |

## 📊 GitHub Actions Pipeline

Jeden `git push` ausgelöst:

```yaml
on:
  push:
    branches: [ "main" ]

jobs:
  build-and-test:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v4
      - name: Kompiliere
        run: g++ *.cpp -o simulator
      - name: Teste
        run: ./simulator
      - name: Static Analysis
        run: cppcheck --enable=all --error-exitcode=1 *.cpp
```

Der Status wird oben als Badge angezeigt! ✨

## 💾 Verwendungsbeispiel

```cpp
LogicEngine engine;
engine.setCircuitName("Beispiel-Schaltung");

// Komponenten hinzufügen
engine.addComponent(std::make_unique<AndGate>("AND-1"));
engine.addComponent(std::make_unique<OrGate>("OR-1"));

// Schaltung simulieren
engine.doTick();
```

## 📚 Weitere Informationen

Dieses Projekt ist Teil des Moduls **Informatik 2 – Labor 7** an der DHBW Stuttgart und behandelt:
- Objektorientiertes Programmieren (OOP)
- Speichermanagement mit Smart Pointers
- DevOps & Continuous Integration
- Automatisierte Testausführung

## 📝 Lizenz

Hochschulprojekt – Verwendung zu Bildungszwecken.

---

**Viel Erfolg beim Programmieren!** 🎓
