#include <iostream>
#include <vector>
#include <memory>
#include "Component.h"
#include "AndGate.h"
#include "OrGate.h"
#include "NotGate.h"
#include "XorGate.h"
#include "NandGate.h"
#include "Switch.h"
#include "DFlipFlop.h"
#include "SimulationEngine.h"

/**
 * =====================================================================
 * Labor 10: Sequenzielle Logik, Takt & das D-Flip-Flop
 * =====================================================================
 * 
 * Diese Demonstration zeigt:
 * 1. Phase 1: Die Klasse DFlipFlop (Das Gedächtnis)
 * 2. Phase 2: Die 2-Phasen-Schleife & DFS-Korrektur
 * 3. Phase 3: Der 1-Bit Zähler (Die Feuerprobe) mit Ringverkabelung
 * 
 * Architektur:
 * - DFlipFlop = Firewall gegen Endlosschleifen
 * - Zwei-Phasen-Simulation:
 *   Phase A (Amnesie): reset() auf alle Gatter
 *   Phase B (Lesen): evaluate() auf Kombinatorik-Endpunkte
 *   Phase C (Ausgabe): Werte drucken
 *   Phase D (Schreiben): onClockTick() auf alle Flip-Flops
 * 
 * Der 1-Bit Zähler:
 *   DFlipFlop ----> NOT ----+
 *   ^                        |
 *   |________________________|
 * 
 * Beim jeden Takt toggelt der Ausgang: 0 -> 1 -> 0 -> 1 ...
 */

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++ Digital Simulator - Labor 10" << std::endl;
    std::cout << "  Sequenzielle Logik & 1-Bit Zähler" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // =====================================================================
    // Phase 1: Die Klasse DFlipFlop (Das Gedächtnis)
    // =====================================================================
    
    std::cout << "[PHASE 1] DFlipFlop-Klasse instanziieren...\n" << std::endl;
    
    auto dff = std::make_shared<DFlipFlop>("Counter-Bit");
    auto notGate = std::make_shared<NotGate>("Toggle");
    
    std::cout << "[INFO] Komponenten erstellt!\n" << std::endl;
    
    // =====================================================================
    // Phase 2: Ringverkabelung - Die Feuerprobe
    // =====================================================================
    
    std::cout << "[PHASE 2] Ringverkabelung (DFF -> NOT -> DFF)...\n" << std::endl;
    
    // DFF-Output als Input zum NOT
    notGate->connectInput(0, dff);
    
    // NOT-Output als Input zum DFF (RINGVERKABELUNG!)
    dff->connectInput(0, notGate);
    
    std::cout << "[INFO] Ring-Schaltkreis vollständig!\n" << std::endl;
    
    // ===== Vektoren für die 2-Phasen-Schleife =====
    std::vector<std::shared_ptr<Gate>> allGates;      // Alle Gatter (für reset)
    std::vector<std::shared_ptr<DFlipFlop>> allFlipFlops;  // Nur Flip-Flops (für Clock)
    
    allGates.push_back(dff);
    allGates.push_back(notGate);
    allFlipFlops.push_back(dff);
    
    // =====================================================================
    // Phase 3: Der 1-Bit Zähler - Die 2-Phasen-Simulation
    // =====================================================================
    
    std::cout << "[PHASE 3] Simulation startet (10 Taktzyklen)...\n" << std::endl;
    std::cout << "┌───────┬─────────────┐" << std::endl;
    std::cout << "│ Takt  │ DFF-Ausgang │" << std::endl;
    std::cout << "├───────┼─────────────┤" << std::endl;
    
    // ===== Schleife über 10 Taktzyklen =====
    for (int cycle = 0; cycle < 10; cycle++) {
        
        // ===== Schritt A: Amnesie - Cache reset =====
        // Alle Gatter "vergessen" ihre bisherigen Berechnungen
        for (auto& gate : allGates) {
            gate->reset();
        }
        
        // ===== Schritt B: Lese-Phase - Kombinatorik berechnen =====
        // WICHTIG: NOT-Gatter muss vor DFF evaluiert werden!
        // DFF ist eine Firewall - es evaluiert seinen Input nicht
        // Daher müssen wir explizit den NOT auswerten
        notGate->evaluate();
        
        // ===== Schritt C: Ausgabe =====
        bool dffOutput = dff->getOutput();
        std::cout << "│ " << cycle << "     │ " << dffOutput << "           │" << std::endl;
        
        // ===== Schritt D: Schreib-Phase - Flip-Flops aktualisieren =====
        // onClockTick() liest den stabilen Output von NOT und speichert ihn
        for (auto& ff : allFlipFlops) {
            ff->onClockTick();
        }
    }
    
    std::cout << "└───────┴─────────────┘" << std::endl;
    std::cout << "\n[SUCCESS] Simulation abgeschlossen!" << std::endl;
    std::cout << "Die Ausgabe sollte abwechselnd 0, 1, 0, 1 sein!" << std::endl;
    
    return 0;
}


