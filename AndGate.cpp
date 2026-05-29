#include "AndGate.h"
#include <iostream>

/**
 * Konstruktor des AndGate
 * Initialisiert die Pins (genau 2 Eingänge) und ruft Basiskonstruktor auf
 */
AndGate::AndGate(std::string n) : Gate(n)
{
    m_inputs.resize(2); // AND-Gatter hat exakt 2 Eingangs-Pins
    std::cout << "[" << m_name << "] AND-Gatter aktiviert (2 Pins)" << std::endl;
}

/**
 * Berechnet die AND-Logik über Smart Pointers (Pull-Prinzip)
 *
 * Floating Pin Check: Sind beide Kabel eingesteckt?
 * - Wenn ja: hole die Werte von den Quell-Gattern ab
 * - Wenn nein: Warnung + Fallback auf false (sicherer Zustand)
 */
void AndGate::evaluate()
{
    if (m_isCalculated)
        return; // Cache Hit! Sofortiger Abbruch der Rekursion.
    // 1. DFS: Vorgänger zwingen, sich zu berechnen!
    if (m_inputs[0] != nullptr)
        m_inputs[0]->evaluate();
    if (m_inputs[1] != nullptr)
        m_inputs[1]->evaluate();
    // 2. Werte sicher auslesen (mit Fallback bei fehlendem Kabel)
    bool valA = (m_inputs[0] != nullptr) ? m_inputs[0]->getOutput() : false;
    bool valB = (m_inputs[1] != nullptr) ? m_inputs[1]->getOutput() : false;
    // 3. Eigene Logik anwenden
    m_output = valA && valB;
    m_isCalculated = true; // Gedächtnis versiegeln
}

/**
 * Gibt den Zustand dieses AND-Gatters aus
 */
void AndGate::printState() const
{
    std::string pinA = (m_inputs[0]) ? "verbunden" : "FLOATING";
    std::string pinB = (m_inputs[1]) ? "verbunden" : "FLOATING";
    std::cout << "AndGate [" << m_name << ": A=" << pinA
              << ", B=" << pinB
              << "] => Output=" << (m_output ? 1 : 0) << std::endl;
}
