#include "NotGate.h"
#include <iostream>

/**
 * Konstruktor des NotGate
 * Initialisiert die Pins (genau 1 Eingang)
 */
NotGate::NotGate(std::string n) : Gate(n)
{
    m_inputs.resize(1); // NOT-Gatter hat exakt 1 Eingangs-Pin
    std::cout << "[" << m_name << "] NOT-Gatter aktiviert (1 Pin)" << std::endl;
}

/**
 * Berechnet die NOT-Logik über Smart Pointers (Pull-Prinzip)
 *
 * Floating Pin Check: Ist das Kabel eingesteckt?
 */
void NotGate::evaluate()
{
    if (m_isCalculated)
        return; // Cache Hit! Sofortiger Abbruch der Rekursion.
    // 1. DFS: Vorgänger zwingen, sich zu berechnen!
    if (m_inputs[0] != nullptr)
        m_inputs[0]->evaluate();
    // 2. Werte sicher auslesen (mit Fallback bei fehlendem Kabel)
    bool valA = (m_inputs[0] != nullptr) ? m_inputs[0]->getOutput() : false;
    // 3. Eigene Logik anwenden
    m_output = !valA;
    m_isCalculated = true; // Gedächtnis versiegeln
}

/**
 * Gibt den Zustand dieses NOT-Gatters aus
 */
void NotGate::printState() const
{
    std::string pin = (m_inputs[0]) ? "verbunden" : "FLOATING";
    std::cout << "NotGate [" << m_name << ": Input=" << pin
              << "] => Output=" << (m_output ? 1 : 0) << std::endl;
}
