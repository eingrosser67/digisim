#include "OrGate.h"
#include <iostream>

/**
 * Konstruktor des OrGate
 * Initialisiert die Pins (genau 2 Eingänge)
 */
OrGate::OrGate(std::string n) : Gate(n) {
    m_inputs.resize(2);  // OR-Gatter hat exakt 2 Eingangs-Pins
    std::cout << "[" << m_name << "] OR-Gatter aktiviert (2 Pins)" << std::endl;
}

/**
 * Berechnet die OR-Logik über Smart Pointers (Pull-Prinzip) mit DFS
 * 
 * Phase 2 (Labor 9): Post-Order DFS Traversal
 * Phase 4 (Labor 9): Memoization (Cache-Flag)
 */
void OrGate::evaluate() {
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
    m_output = valA || valB;
    
    // ===== Phase 4: Cache-Flag setzen =====
    m_isCalculated = true;
}

/**
 * Gibt den Zustand dieses OR-Gatters aus
 */
void OrGate::printState() const {
    std::string pinA = (m_inputs[0]) ? "verbunden" : "FLOATING";
    std::string pinB = (m_inputs[1]) ? "verbunden" : "FLOATING";
    std::cout << "OrGate [" << m_name << ": A=" << pinA 
              << ", B=" << pinB 
              << "] => Output=" << (m_output ? 1 : 0) << std::endl;
}
