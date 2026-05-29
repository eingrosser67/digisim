#include "NotGate.h"
#include <iostream>

/**
 * Konstruktor des NotGate
 * Initialisiert die Pins (genau 1 Eingang)
 */
NotGate::NotGate(std::string n) : Gate(n) {
    m_inputs.resize(1);  // NOT-Gatter hat exakt 1 Eingangs-Pin
    std::cout << "[" << m_name << "] NOT-Gatter aktiviert (1 Pin)" << std::endl;
}

/**
 * Berechnet die NOT-Logik über Smart Pointers (Pull-Prinzip) mit DFS
 * 
 * Phase 2 (Labor 9): Post-Order DFS Traversal
 * Phase 4 (Labor 9): Memoization (Cache-Flag)
 */
void NotGate::evaluate() {
    // ===== Phase 4: Cache-Abfrage =====
    if (m_isCalculated) {
        return;  // Cache Hit!
    }
    
    // ===== Phase 2, Schritt A: DFS - Vorgänger evaluieren =====
    if (m_inputs[0] != nullptr) {
        m_inputs[0]->evaluate();
    }
    
    // ===== Phase 2, Schritt B: Wert auslesen =====
    bool val = (m_inputs[0] != nullptr) ? m_inputs[0]->getOutput() : false;
    
    // ===== Phase 2, Schritt C: Logik anwenden =====
    m_output = !val;
    
    // ===== Phase 4: Cache-Flag setzen =====
    m_isCalculated = true;
}

/**
 * Gibt den Zustand dieses NOT-Gatters aus
 */
void NotGate::printState() const {
    std::string pin = (m_inputs[0]) ? "verbunden" : "FLOATING";
    std::cout << "NotGate [" << m_name << ": Input=" << pin 
              << "] => Output=" << (m_output ? 1 : 0) << std::endl;
}





