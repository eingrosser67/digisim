#include "NandGate.h"
#include <iostream>

/**
 * Konstruktor des NandGate
 * Initialisiert die Pins (genau 2 Eingänge)
 */
NandGate::NandGate(std::string n) : Gate(n) {
    m_inputs.resize(2);  // NAND-Gatter hat exakt 2 Eingangs-Pins
    std::cout << "[" << m_name << "] NAND-Gatter aktiviert (2 Pins)" << std::endl;
}

/**
 * Berechnet die NAND-Logik über Smart Pointers (Pull-Prinzip) mit DFS
 * NAND = NOT(AND) = !(A && B)
 * 
 * Phase 2 (Labor 9): Post-Order DFS Traversal
 * Phase 4 (Labor 9): Memoization (Cache-Flag)
 */
void NandGate::evaluate() {
    // ===== Phase 4: Cache-Abfrage =====
    if (m_isCalculated) {
        return;  // Cache Hit!
    }
    
    // ===== Phase 2, Schritt A: DFS - Vorgänger evaluieren =====
    if (m_inputs[0] != nullptr) {
        m_inputs[0]->evaluate();
    }
    if (m_inputs[1] != nullptr) {
        m_inputs[1]->evaluate();
    }
    
    // ===== Phase 2, Schritt B: Werte auslesen =====
    bool valA = (m_inputs[0] != nullptr) ? m_inputs[0]->getOutput() : false;
    bool valB = (m_inputs[1] != nullptr) ? m_inputs[1]->getOutput() : false;
    
    // ===== Phase 2, Schritt C: Logik anwenden =====
    m_output = !(valA && valB);
    
    // ===== Phase 4: Cache-Flag setzen =====
    m_isCalculated = true;
}

/**
 * Gibt den Zustand dieses NAND-Gatters aus
 */
void NandGate::printState() const {
    std::string pinA = (m_inputs[0]) ? "verbunden" : "FLOATING";
    std::string pinB = (m_inputs[1]) ? "verbunden" : "FLOATING";
    std::cout << "NandGate [" << m_name << ": A=" << pinA 
              << ", B=" << pinB 
              << "] => Output=" << (m_output ? 1 : 0) << std::endl;
}
