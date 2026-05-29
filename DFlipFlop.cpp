#include "DFlipFlop.h"
#include <iostream>

/**
 * Konstruktor des DFlipFlop
 * Initialisiert die Pin (genau 1 Eingang)
 */
DFlipFlop::DFlipFlop(std::string n) : Gate(n) {
    m_inputs.resize(1);  // D-Flip-Flop hat exakt 1 Eingangs-Pin
    m_storedState = false;
    std::cout << "[" << m_name << "] D-Flip-Flop aktiviert (1 Pin, initial=0)" << std::endl;
}

/**
 * Firewall-Methode: NICHT den Input evaluieren!
 * 
 * Dies ist die Kernidee des D-Flip-Flops:
 * - Die evaluate()-Methode wird in der Lese-Phase aufgerufen
 * - Sie gibt den ALTEN Wert (m_storedState) zurück
 * - Sie ruft evaluate() auf dem Input NICHT auf (Firewall!)
 * - Dadurch wird eine Rekursion durchbrochen
 * 
 * Beispiel Ringverkabelung:
 *   DFF -> NOT -> DFF
 * 
 * Wenn NOT->evaluate() den DFF evaluiert:
 * - DFF->evaluate() würde normalerweise NOT->evaluate() aufrufen
 * - Das würde Endlosschleife erzeugen!
 * - Aber DFF gibt nur m_storedState zurück (Firewall)
 * - So wird die Rekursion durchbrochen
 */
void DFlipFlop::evaluate() {
    // ===== Cache-Abfrage =====
    if (m_isCalculated) {
        return;
    }
    
    // ===== Firewall: Input wird NICHT evaluiert! =====
    // Wir geben einfach den gespeicherten Wert zurück
    // Dies ist die Firewall gegen Endlosschleifen!
    
    // ===== Ausgang setzen auf gespeicherten Wert =====
    m_output = m_storedState;
    
    // ===== Cache-Flag setzen =====
    m_isCalculated = true;
}

/**
 * Taktflanke-Simulation
 * 
 * Diese Methode wird in der Schreib-Phase aufgerufen
 * (nach allen evaluate()-Aufrufen der Lese-Phase)
 * 
 * Sie:
 * 1. Liest den aktuellen Input-Wert (der Input wurde bereits in der Lese-Phase berechnet)
 * 2. Speichert ihn in m_storedState (der Tresor)
 * 3. Bei der nächsten Lese-Phase gibt evaluate() diesen neuen Wert zurück
 */
void DFlipFlop::onClockTick() {
    // Lese den Input-Wert
    bool inputValue = (m_inputs[0] != nullptr) ? m_inputs[0]->getOutput() : false;
    
    // Speichere ihn im Tresor
    m_storedState = inputValue;
    
    std::cout << "[CLOCK] " << m_name << " speichert Input: " << inputValue 
              << " (gespeicherter Zustand = " << m_storedState << ")" << std::endl;
}

/**
 * Gibt den Zustand dieses D-Flip-Flops aus
 */
void DFlipFlop::printState() const {
    std::string pin = (m_inputs[0]) ? "verbunden" : "FLOATING";
    std::cout << "DFlipFlop [" << m_name << ": Input=" << pin 
              << ", Stored=" << (m_storedState ? 1 : 0)
              << "] => Output=" << (m_output ? 1 : 0) << std::endl;
}

/**
 * Virtueller Destruktor
 */
DFlipFlop::~DFlipFlop() {
    std::cout << "[DESTRUKTOR] Zerstöre D-Flip-Flop: " << m_name << std::endl;
}
