#include "AndGate.h"
#include <iostream>

/**
 * Konstruktor des AndGate
 * Initialisiert die Pins (genau 2 Eingänge) und ruft Basiskonstruktor auf
 */
AndGate::AndGate(std::string n) : Gate(n) {
    m_inputs.resize(2);  // AND-Gatter hat exakt 2 Eingangs-Pins
    std::cout << "[" << m_name << "] AND-Gatter aktiviert (2 Pins)" << std::endl;
}

/**
 * Berechnet die AND-Logik über Smart Pointers (Pull-Prinzip) mit DFS
 * 
 * Phase 2 (Labor 9): Post-Order DFS Traversal
 * - Schritt A: Rekursive evaluate() auf allen Inputs aufrufen
 * - Schritt B: Werte via getOutput() auslesen
 * - Schritt C: Eigene Logik berechnen
 * 
 * Phase 4 (Labor 9): Memoization (Cache-Flag)
 * - Cache-Hit sofort zurückkehren (Zeitoptimierung)
 * - Cache-Flag am Ende setzen (Speichern des Ergebnisses)
 */
void AndGate::evaluate() {
    // ===== Phase 4: Cache-Abfrage (sofort zurück, wenn schon berechnet) =====
    if (m_isCalculated) {
        return;  // Cache Hit! Keine redundante Berechnung
    }
    
    // ===== Phase 2, Schritt A: DFS - Vorgänger zwingen, sich zu berechnen =====
    if (m_inputs[0] != nullptr) {
        m_inputs[0]->evaluate();
    }
    if (m_inputs[1] != nullptr) {
        m_inputs[1]->evaluate();
    }
    
    // ===== Phase 2, Schritt B: Werte mit Fallback (Floating-Pin-Check) =====
    bool valA = (m_inputs[0] != nullptr) ? m_inputs[0]->getOutput() : false;
    bool valB = (m_inputs[1] != nullptr) ? m_inputs[1]->getOutput() : false;
    
    // ===== Phase 2, Schritt C: Eigene Logik anwenden =====
    m_output = valA && valB;
    
    // ===== Phase 4: Cache-Flag setzen (Gedächtnis versiegeln) =====
    m_isCalculated = true;
}

/**
 * Gibt den Zustand dieses AND-Gatters aus
 */
void AndGate::printState() const {
    std::string pinA = (m_inputs[0]) ? "verbunden" : "FLOATING";
    std::string pinB = (m_inputs[1]) ? "verbunden" : "FLOATING";
    std::cout << "AndGate [" << m_name << ": A=" << pinA 
              << ", B=" << pinB 
              << "] => Output=" << (m_output ? 1 : 0) << std::endl;
}
