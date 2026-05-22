# Logik-Simulator 🔌

Ein **C++-basierter Simulator für digitale Logikgatter** - ideal zum Verständnis von Schaltlogik, Speichermanagement und objektorientierten Designmustern.

## 📋 Überblick

Dieses Projekt implementiert einen vollständigen Logik-Simulator mit verschiedenen digitalen Gattern:

- **AND-Gate** – Logisches UND
- **OR-Gate** – Logisches ODER  
- **NOT-Gate** – Logische Negation
- **XOR-Gate** – Exklusives ODER
- **NAND-Gate** – Negiertes UND

## 🏗️ Projektstruktur

```
├── Component.h/cpp        # Abstrakte Basisklasse für alle Gatter
├── LogicEngine.h/cpp      # Kern-Engine zur Verwaltung von Schaltungen
├── AndGate.h/cpp          # AND-Gatter-Implementierung
├── OrGate.h/cpp           # OR-Gatter-Implementierung
├── NotGate.h/cpp          # NOT-Gatter-Implementierung
├── XorGate.h/cpp          # XOR-Gatter-Implementierung
├── NandGate.h/cpp         # NAND-Gatter-Implementierung
├── main.cpp               # Beispiel-Anwendung
└── README.md              # Diese Datei
```

## 🎯 Kernkonzepte

### Polymorphismus
Alle Gatter erben von der abstrakten Basisklasse `Component` und implementieren die virtuelle Methode `update()`. Die `LogicEngine` verwaltet die Komponenten polymorphisch über `std::unique_ptr<Component>`.

### Speichermanagement
Das Projekt demonstriert modernes C++ Speichermanagement mit:
- **Smart Pointers** (`std::unique_ptr`) statt rohe Zeiger
- Automatische Speicherfreigabe ohne Memory Leaks
- RAII-Prinzip (Resource Acquisition Is Initialization)

### Observer-Pattern
Komponenten können über Zeiger beobachtet und ausgelesen werden, ohne das Objekt zu kopieren.

## 🚀 Kompilierung & Ausführung

### Voraussetzungen
- C++17 oder höher
- Ein C++-Compiler (GCC, Clang, MSVC)

### Kompilieren
```bash
g++ -std=c++17 -o simulator *.cpp
```

### Ausführen
```bash
./simulator
```

## 💾 Verwendungsbeispiel

```cpp
LogicEngine engine;
engine.setCircuitName("Beispiel-Schaltung");

// Komponenten hinzufügen
engine.addComponent(std::make_unique<AndGate>("AND-1"));
engine.addComponent(std::make_unique<OrGate>("OR-1"));

// Schaltung simulieren
engine.simulate(10);  // 10 Takte simulieren
```

## 📚 Weitere Informationen

Dieses Projekt ist Teil des Moduls **Informatik 2** an der DHBW Stuttgart und behandelt fortgeschrittene C++ Konzepte im praktischen Kontext einer Logik-Simulation.

## 📝 Lizenz

Hochschulprojekt – Verwendung zu Bildungszwecken.

---

**Viel Erfolg beim Programmieren!** 🎓
