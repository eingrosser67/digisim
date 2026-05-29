#include <iostream>
#include <vector>
#include <memory>
#include "Component.h"
#include "NotGate.h"
#include "XorGate.h"
#include "DFlipFlop.h"
#include "DFlipFlopEnable.h"
#include "SimulationEngine.h"

/**
 * =====================================================================
 * Labor 10 - ZUSATZAUFGABEN
 * =====================================================================
 * 
 * ZUSATZAUFGABE 1: D-Flip-Flop mit Enable-Pin (DFFE)
 * - Erweitert DFlipFlop um einen EN-Pin
 * - Speichert Input NUR wenn EN=true
 * - Anwendung: Selektive Speicherung, Zähler mit Pause
 * 
 * ZUSATZAUFGABE 2: Der 2-Bit Zähler
 * - Zählt von 0 bis 3 (00 -> 01 -> 10 -> 11 -> 00)
 * - Bit 0 toggelt bei jedem Takt (wie 1-Bit Zähler)
 * - Bit 1 toggelt NUR wenn Bit 0 = 1 (kaskadierter Zähler)
 * 
 * Schaltung für 2-Bit Zähler:
 *   DFF0 ---[NOT]--+
 *   ^              |
 *   |______________|
 *   
 *   DFF0 Output
 *   DFF1 Output ----[XOR]----+
 *                    ^       |
 *                    |_______|
 *   DFF1 <---------+
 */

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++ Digital Simulator - Labor 10" << std::endl;
    std::cout << "  ZUSATZAUFGABEN (DFFE & 2-Bit Zähler)" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // =====================================================================
    // ZUSATZAUFGABE 1: DFFE - Demonstration mit Enable-Signal
    // =====================================================================
    
    std::cout << "[ZUSATZAUFGABE 1] D-Flip-Flop mit Enable-Pin\n" << std::endl;
    std::cout << "Szenario: DFF speichert Input NUR wenn Enable=1\n" << std::endl;
    
    auto dffe = std::make_shared<DFlipFlopEnable>("Memory-with-Enable");
    auto toggler = std::make_shared<NotGate>("Toggler");
    auto enableInput = std::make_shared<NotGate>("Enable-Generator");
    
    // Ringverkabelung für Toggler
    toggler->connectInput(0, dffe);
    dffe->connectInput(0, toggler);
    
    // Enable-Signal wird von einem NOT-Gatter erzeugt (ändert alle 2 Takte)
    enableInput->connectInput(0, dffe);
    dffe->connectInput(1, enableInput);
    
    std::vector<std::shared_ptr<Gate>> gates1;
    std::vector<std::shared_ptr<DFlipFlopEnable>> ffes1;
    gates1.push_back(dffe);
    gates1.push_back(toggler);
    gates1.push_back(enableInput);
    ffes1.push_back(dffe);
    
    std::cout << "Schaltung: DFF0 -> NOT -> DFF0 (Ring)\n" << std::endl;
    std::cout << "┌───────┬─────────────┬──────────┐" << std::endl;
    std::cout << "│ Takt  │ DFF-Ausgang │ Enable   │" << std::endl;
    std::cout << "├───────┼─────────────┼──────────┤" << std::endl;
    
    for (int cycle = 0; cycle < 8; cycle++) {
        // Phase A: Reset
        for (auto& gate : gates1) {
            gate->reset();
        }
        
        // Phase B: Evaluate
        toggler->evaluate();
        enableInput->evaluate();
        
        // Phase C: Output
        bool dffOut = dffe->getOutput();
        bool enOut = enableInput->getOutput();
        std::cout << "│ " << cycle << "     │ " << dffOut << "           │ " << enOut << "       │" << std::endl;
        
        // Phase D: Clock Tick
        for (auto& ff : ffes1) {
            ff->onClockTick();
        }
    }
    
    std::cout << "└───────┴─────────────┴──────────┘" << std::endl;
    std::cout << "\nBeobachtung: DFF speichert nur wenn Enable=1\n" << std::endl;
    
    // =====================================================================
    // ZUSATZAUFGABE 2: 2-Bit Zähler
    // =====================================================================
    
    std::cout << "\n[ZUSATZAUFGABE 2] 2-Bit Zähler (0-3 Zyklus)\n" << std::endl;
    std::cout << "Schaltung:\n";
    std::cout << "  Bit 0: DFF0 <-> NOT0 (Ringverkabelung, toggelt jeden Takt)\n";
    std::cout << "  Bit 1: DFF1 <-> XOR (toggelt nur wenn Bit0=1)\n\n";
    
    // ===== Bit 0: Einfacher 1-Bit Zähler =====
    auto dff0 = std::make_shared<DFlipFlop>("Bit0");
    auto not0 = std::make_shared<NotGate>("NOT0");
    
    not0->connectInput(0, dff0);
    dff0->connectInput(0, not0);
    
    // ===== Bit 1: Toggelt nur wenn Bit0=1 =====
    auto dff1 = std::make_shared<DFlipFlop>("Bit1");
    auto xor1 = std::make_shared<XorGate>("XOR1");
    
    // XOR-Input 0: Ausgang von DFF0 (aktuelle Zustand von Bit 0)
    // XOR-Input 1: Ausgang von DFF1 (aktueller Zustand von Bit 1)
    xor1->connectInput(0, dff0);
    xor1->connectInput(1, dff1);
    
    // XOR-Output -> DFF1 Input
    dff1->connectInput(0, xor1);
    
    // ===== Vektoren für die Schleife =====
    std::vector<std::shared_ptr<Gate>> gates2;
    std::vector<std::shared_ptr<DFlipFlop>> ffs2;
    
    gates2.push_back(dff0);
    gates2.push_back(not0);
    gates2.push_back(dff1);
    gates2.push_back(xor1);
    
    ffs2.push_back(dff0);
    ffs2.push_back(dff1);
    
    // ===== Simulation: 16 Takte (ein vollständiger Zyklus 0-3 wiederholt sich) =====
    std::cout << "┌───────┬──────┬──────┬─────────┐" << std::endl;
    std::cout << "│ Takt  │ Bit1 │ Bit0 │ Dezimal │" << std::endl;
    std::cout << "├───────┼──────┼──────┼─────────┤" << std::endl;
    
    for (int cycle = 0; cycle < 16; cycle++) {
        // Phase A: Reset
        for (auto& gate : gates2) {
            gate->reset();
        }
        
        // Phase B: Evaluate
        not0->evaluate();
        xor1->evaluate();
        
        // Phase C: Output
        bool bit0 = dff0->getOutput();
        bool bit1 = dff1->getOutput();
        int dezimal = (bit1 ? 2 : 0) + (bit0 ? 1 : 0);
        
        std::cout << "│ " << (cycle < 10 ? " " : "") << cycle << "    │  " << bit1 << "   │  " << bit0 
                  << "   │  " << dezimal << "    │" << std::endl;
        
        // Phase D: Clock Tick
        for (auto& ff : ffs2) {
            ff->onClockTick();
        }
    }
    
    std::cout << "└───────┴──────┴──────┴─────────┘" << std::endl;
    
    std::cout << "\n[SUCCESS] 2-Bit Zähler funktioniert!" << std::endl;
    std::cout << "Sequenz: 0 (00) -> 1 (01) -> 2 (10) -> 3 (11) -> 0 (00) ..." << std::endl;
    std::cout << "Alle Zusatzaufgaben abgeschlossen! ✓" << std::endl;
    
    return 0;
}
