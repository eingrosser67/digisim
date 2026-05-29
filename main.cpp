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
#include "SimulationEngine.h"

/**
 * =====================================================================
 * Labor 9: Traversierung, Rekursion & Optimierung
 * =====================================================================
 * 
 * Diese Demonstration zeigt:
 * 1. Phase 1: Der Rekursionsanker (Switch.evaluate() bleibt leer)
 * 2. Phase 2: Post-Order DFS in allen Gattern
 * 3. Phase 3: Volladdierer (Full Adder) mit zwei Halbaddierern
 * 4. Phase 4: Memoization (Cache-Flag) & reset() bei jeder Iteration
 * 
 * Ein Volladdierer:
 * - Nimmt DREI Ein-Bit-Eingänge (A, B, Cin)
 * - Produziert zwei Ausgänge: Summe (S) und Carry Out (Cout)
 * - S = A XOR B XOR Cin
 * - Cout = (A AND B) OR ((A XOR B) AND Cin)
 * 
 * Interner Aufbau mittels zwei Halbaddierern:
 * - HA1: XOR_1 und AND_1 verknüpfen A und B
 * - HA2: XOR_2 und AND_2 verknüpfen Sum1 (von XOR_1) und Cin
 * - Finales OR kombiniert die Carries (Cout = Carry1 OR Carry2)
 */

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++ Digital Simulator - Labor 9" << std::endl;
    std::cout << "  Volladdierer (Full Adder) mit DFS & Memoization" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // =====================================================================
    // Phase 3: Volladdierer montieren
    // =====================================================================
    
    std::cout << "[SCHRITT 1] Komponenten instanziieren...\n" << std::endl;
    
    // ===== Datenquellen (Schalter) =====
    auto swA = std::make_shared<Switch>("Input A");
    auto swB = std::make_shared<Switch>("Input B");
    auto swCin = std::make_shared<Switch>("Carry In");
    
    // ===== Halbaddierer 1 (HA1): A und B verknüpfen =====
    auto xor_1 = std::make_shared<XorGate>("XOR_1 (HA1-Sum)");
    auto and_1 = std::make_shared<AndGate>("AND_1 (HA1-Carry)");
    
    // ===== Halbaddierer 2 (HA2): Sum1 und Cin verknüpfen =====
    auto xor_2 = std::make_shared<XorGate>("XOR_2 (HA2-Sum/Final)");
    auto and_2 = std::make_shared<AndGate>("AND_2 (HA2-Carry)");
    
    // ===== Finales Carry-Out (OR kombiniert die Carries) =====
    auto or_final = std::make_shared<OrGate>("OR (Final Carry Out)");
    
    // ===== Phase 4: Alle Gatter bei der Engine registrieren =====
    SimulationEngine engine;
    engine.addGate(swA);
    engine.addGate(swB);
    engine.addGate(swCin);
    engine.addGate(xor_1);
    engine.addGate(and_1);
    engine.addGate(xor_2);
    engine.addGate(and_2);
    engine.addGate(or_final);
    std::cout << "[INFO] " << engine.gateCount() << " Komponenten bei Engine registriert\n" << std::endl;
    
    std::cout << "[SCHRITT 2] Volladdierer-Schaltkreis verkabeln...\n" << std::endl;
    
    // ===== Halbaddierer 1: A und B an XOR_1 und AND_1 =====
    xor_1->connectInput(0, swA);    // XOR_1 Pin 0 = Input A
    xor_1->connectInput(1, swB);    // XOR_1 Pin 1 = Input B
    
    and_1->connectInput(0, swA);    // AND_1 Pin 0 = Input A
    and_1->connectInput(1, swB);    // AND_1 Pin 1 = Input B
    
    // ===== Halbaddierer 2: XOR_1 Output und Cin an XOR_2 und AND_2 =====
    xor_2->connectInput(0, xor_1);  // XOR_2 Pin 0 = XOR_1 Output (Sum1)
    xor_2->connectInput(1, swCin);  // XOR_2 Pin 1 = Carry In
    
    and_2->connectInput(0, xor_1);  // AND_2 Pin 0 = XOR_1 Output
    and_2->connectInput(1, swCin);  // AND_2 Pin 1 = Carry In
    
    // ===== Finales Carry: AND_1 und AND_2 am OR =====
    or_final->connectInput(0, and_1);  // OR Pin 0 = AND_1 Output (Carry1)
    or_final->connectInput(1, and_2);  // OR Pin 1 = AND_2 Output (Carry2)
    
    std::cout << "[INFO] Schaltkreis verkabelt!\n" << std::endl;
    
    std::cout << "[SCHRITT 3] Wahrheitstabelle: Alle 8 Kombinationen testen...\n" << std::endl;
    std::cout << "┌─────┬─────┬─────┬────────┬────────┐" << std::endl;
    std::cout << "│  A  │  B  │ Cin │  Sum   │ Cout   │" << std::endl;
    std::cout << "├─────┼─────┼─────┼────────┼────────┤" << std::endl;
    
    // Test-Kombinationen: 0+0+0 bis 1+1+1 (8 Fälle)
    std::vector<std::tuple<bool, bool, bool>> testCases = {
        {false, false, false},
        {false, false, true},
        {false, true, false},
        {false, true, true},
        {true, false, false},
        {true, false, true},
        {true, true, false},
        {true, true, true}
    };
    
    int testCount = 0;
    int passedCount = 0;
    
    for (auto [a, b, cin] : testCases) {
        // ===== Phase 4: KRITISCH - Alle Caches leeren bevor neue Berechnung =====
        engine.resetAll();
        
        // ===== Schalter neu setzen =====
        swA->setState(a);
        swB->setState(b);
        swCin->setState(cin);
        
        // ===== Phase 2/3: Magischer Moment - DFS Pull-Prinzip! =====
        // Wir rufen evaluate() NUR am Ausgang auf. Alles andere zieht sich die Engine selbst!
        xor_2->evaluate();      // Zieht sich von xor_1, which zieht sich von swA/swB
        or_final->evaluate();   // Zieht sich von and_1/and_2, which ziehen sich von swA/swB/swCin
        
        // ===== Ergebnisse auslesen =====
        bool sum = xor_2->getOutput();
        bool cout = or_final->getOutput();
        
        // ===== Berechne erwartete Werte (Wahrheitstabelle Volladdierer) =====
        int sum_expected = a ^ b ^ cin;        // XOR XOR (Summe)
        int cout_expected = (a & b) | ((a ^ b) & cin);  // (AND) OR (AND)
        
        // ===== Ausgabe der Testzelle =====
        std::cout << "│  " << (a ? 1 : 0) << "  │  " << (b ? 1 : 0) << "  │  "
                  << (cin ? 1 : 0) << "  │";
        std::cout << "   " << (sum ? 1 : 0) << "    │   " << (cout ? 1 : 0) << "    │";
        
        // ===== Überprüfe Korrektheit =====
        testCount += 2;
        if (sum == sum_expected) {
            std::cout << " ✓";
            passedCount++;
        } else {
            std::cout << " ✗";
        }
        std::cout << " ";
        if (cout == cout_expected) {
            std::cout << "✓ " << std::endl;
            passedCount++;
        } else {
            std::cout << "✗ " << std::endl;
        }
    }
    
    std::cout << "└─────┴─────┴─────┴────────┴────────┘" << std::endl;
    
    std::cout << "\n[SCHRITT 4] Zustandsbericht der Komponenten (letzte Iteration):\n" << std::endl;
    
    swA->printState();
    swB->printState();
    swCin->printState();
    xor_1->printState();
    and_1->printState();
    xor_2->printState();
    and_2->printState();
    or_final->printState();
    
    // =====================================================================
    // Abschluss und Exit-Code
    // =====================================================================
    std::cout << "\n========================================" << std::endl;
    std::cout << "Test Summary:" << std::endl;
    std::cout << "Bestanden: " << passedCount << " / " << testCount << std::endl;
    std::cout << "========================================" << std::endl;

    if (passedCount == testCount) {
        std::cout << "\n[SUCCESS] Volladdierer funktioniert korrekt! ✓" << std::endl;
        std::cout << "Phase 2 (DFS): Gatter ziehen sich Werte selbst" << std::endl;
        std::cout << "Phase 3 (Full Adder): Zwei HA + OR-Gatter" << std::endl;
        std::cout << "Phase 4 (Memoization): Cache-Flag optimiert O(2^n) → O(n)" << std::endl;
        return 0;  // ← EXIT-CODE 0: ERFOLG
    } else {
        std::cerr << "\n[FEHLER] Mindestens ein Test fehlgeschlagen!" << std::endl;
        return 1;  // ← EXIT-CODE 1: FEHLER
    }
}


