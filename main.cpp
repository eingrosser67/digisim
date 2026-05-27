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

/**
 * =====================================================================
 * Labor 7: Schaltkreisaufbau mit Smart Pointers (DAG-Architektur)
 * =====================================================================
 * 
 * Diese Demonstration zeigt:
 * 1. Den Aufbau eines echten digitalen Schaltkreises
 * 2. Die Verbindung von Gattern über Smart Pointers (Kupferkabel)
 * 3. Die Simulation eines Halbaddierers (Half Adder)
 * 4. Das "Pull-Prinzip" für Werteabfrage
 * 
 * Ein Halbaddierer:
 * - Nimmt zwei Ein-Bit-Eingänge (A, B)
 * - Produziert zwei Ausgänge: Summe (S) und Carry (C)
 * - S = A XOR B
 * - C = A AND B
 */

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++ Digital Simulator - Labor 7" << std::endl;
    std::cout << "  Halbaddierer (Half Adder) Schaltkreis" << std::endl;
    std::cout << "========================================\n" << std::endl;

    // =====================================================================
    // Phase 4: Halbaddierer montieren
    // =====================================================================
    
    std::cout << "[SCHRITT 1] Komponenten instanziieren (als Smart Pointers)...\n" << std::endl;
    
    // Datenquellen (Schalter)
    auto swA = std::make_shared<Switch>("Input A");
    auto swB = std::make_shared<Switch>("Input B");
    
    // Logikgatter
    auto xorGate = std::make_shared<XorGate>("XOR (Summe)");
    auto andGate = std::make_shared<AndGate>("AND (Carry)");
    
    std::cout << "\n[SCHRITT 2] Schaltkreis verkabeln (DAG-Aufbau)...\n" << std::endl;
    
    // Verkabelung (Fan-Out!): Beide Schalter gehen an beide Gatter
    // ┌─────────────┐
    // │  Input A ──┬──> XOR
    // └─────────────┤
    //              └──> AND
    //
    // ┌─────────────┐
    // │  Input B ──┬──> XOR
    // └─────────────┤
    //              └──> AND
    
    xorGate->connectInput(0, swA);    // XOR Pin 0 = Input A
    xorGate->connectInput(1, swB);    // XOR Pin 1 = Input B
    
    andGate->connectInput(0, swA);    // AND Pin 0 = Input A
    andGate->connectInput(1, swB);    // AND Pin 1 = Input B
    
    std::cout << "\n[SCHRITT 3] Wahrheitstabelle: Alle 4 Kombinationen testen...\n" << std::endl;
    std::cout << "┌─────┬─────┬────────┬───────┐" << std::endl;
    std::cout << "│  A  │  B  │ Summe  │ Carry │" << std::endl;
    std::cout << "├─────┼─────┼────────┼───────┤" << std::endl;
    
    // Test-Kombinationen: 0+0, 0+1, 1+0, 1+1
    std::vector<std::pair<bool, bool>> testCases = {
        {false, false},
        {false, true},
        {true, false},
        {true, true}
    };
    
    int testCount = 0;
    int passedCount = 0;
    
    for (auto [a, b] : testCases) {
        // KRITISCH: Erst Schalter setzen, DANN evaluate() aufrufen!
        swA->setState(a);
        swB->setState(b);
        
        // Evaluiere die Gatter (Pull-Prinzip: Sie holen sich Werte selbst)
        xorGate->evaluate();
        andGate->evaluate();
        
        // Lese die Ergebnisse aus
        bool summe = xorGate->getOutput();
        bool carry = andGate->getOutput();
        
        // Berechne erwartete Werte
        bool expectedSum = a ^ b;        // XOR
        bool expectedCarry = a && b;     // AND
        
        // Ausgabe der Testzelle
        std::cout << "│  " << (a ? 1 : 0) << "  │  " << (b ? 1 : 0) << "  │";
        std::cout << "   " << (summe ? 1 : 0) << "    │";
        std::cout << "   " << (carry ? 1 : 0) << "   │";
        
        // Überprüfe Korrektheit
        testCount += 2;
        if (summe == expectedSum) {
            std::cout << " ✓";
            passedCount++;
        } else {
            std::cout << " ✗";
        }
        std::cout << " ";
        if (carry == expectedCarry) {
            std::cout << "✓ " << std::endl;
            passedCount++;
        } else {
            std::cout << "✗ " << std::endl;
        }
    }
    
    std::cout << "└─────┴─────┴────────┴───────┘" << std::endl;
    
    std::cout << "\n[SCHRITT 4] Zustandsbericht der Komponenten:\n" << std::endl;
    
    swA->printState();
    swB->printState();
    xorGate->printState();
    andGate->printState();
    
    // =====================================================================
    // Abschluss und Exit-Code
    // =====================================================================
    std::cout << "\n========================================" << std::endl;
    std::cout << "Test Summary:" << std::endl;
    std::cout << "Bestanden: " << passedCount << " / " << testCount << std::endl;
    std::cout << "========================================" << std::endl;

    if (passedCount == testCount) {
        std::cout << "\n[SUCCESS] Halbaddierer funktioniert korrekt! ✓" << std::endl;
        std::cout << "Der DAG wurde erfolgreich aufgebaut und evaluiert." << std::endl;
        return 0;  // ← EXIT-CODE 0: ERFOLG (Grüner Haken für CI)
    } else {
        std::cerr << "\n[FEHLER] Mindestens ein Test fehlgeschlagen!" << std::endl;
        return 1;  // ← EXIT-CODE 1: FEHLER (Rotes Kreuz für CI)
    }
}







