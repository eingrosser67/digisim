#include "DFlipFlop.h"
#include <iostream>

DFlipFlop::DFlipFlop(std::string n) : Gate(n)
{
    m_inputs.resize(1); // D-FlipFlop hat 1 Eingang: D
    std::cout << "[" << m_name << "] D-FlipFlop aktiviert (1 Pin: D)" << std::endl;
}

void DFlipFlop::evaluate()
{
    // Firewall: Keine Rückwärtsauswertung des Eingangsgatters.
    // Der Ausgang entspricht dem zuletzt bei einem Clock-Tick gespeicherten Zustand.
    m_output = m_storedState;
    m_isCalculated = true;
}

void DFlipFlop::onClockTick()
{
    if (m_inputs[0])
    {
        // Den aktuellen Eingangswert übernehmen, ohne evaluate() am Eingang aufzurufen.
        m_storedState = m_inputs[0]->getOutput();
    }
    else
    {
        m_storedState = false;
    }
}

void DFlipFlop::printState() const
{
    std::string pinD = (m_inputs[0]) ? "verbunden" : "FLOATING";
    std::cout << "DFlipFlop [" << m_name << ": D=" << pinD
              << "] => Q=" << (m_output ? 1 : 0)
              << ", stored=" << (m_storedState ? 1 : 0) << std::endl;
}