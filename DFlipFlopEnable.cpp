#include "DFlipFlopEnable.h"
#include <iostream>

/**
 * Konstruktor des DFlipFlopEnable
 * Initialisiert die Pins (genau 2 Eingänge: D und EN)
 */
DFlipFlopEnable::DFlipFlopEnable(std::string n) : Gate(n) {
    m_inputs.resize(2);  // DFFE hat exakt 2 Eingangs-Pins
    m_storedState = false;
    std::cout << "[" << m_name << "] D-Flip-Flop mit Enable aktiviert (2 Pins: D, EN | initial=0)" << std::endl;
}

/**
 * Firewall-Methode: Gibt den gespeicherten Wert zurück
 */
void DFlipFlopEnable::evaluate() {
    // ===== Cache-Abfrage =====
    if (m_isCalculated) {
        return;
    }
    
    // ===== Firewall: Input wird NICHT evaluiert! =====
    // Wir geben einfach den gespeicherten Wert zurück
    
    // ===== Ausgang setzen auf gespeicherten Wert =====
    m_output = m_storedState;
    
    // ===== Cache-Flag setzen =====
    m_isCalculated = true;
}

/**
 * Taktflanke-Simulation mit Enable
 * 
 * Diese Methode wird in der Schreib-Phase aufgerufen
 * 
 * Sie:
 * 1. Liest Pin 1 (Enable-Signal)
 * 2. Wenn EN=true: Liest Pin 0 (D) und speichert in m_storedState
 * 3. Wenn EN=false: Speichert nichts (Zustand bleibt gleich)
 */
void DFlipFlopEnable::onClockTick() {
    // ===== Lese Enable-Signal (Pin 1) =====
    bool enableSignal = (m_inputs[1] != nullptr) ? m_inputs[1]->getOutput() : false;
    
    // ===== Lese Daten (Pin 0) =====
    bool dataValue = (m_inputs[0] != nullptr) ? m_inputs[0]->getOutput() : false;
    
    // ===== Speichere NUR wenn Enable=true =====
    if (enableSignal) {
        m_storedState = dataValue;
        std::cout << "[CLOCK] " << m_name << " (EN=1) speichert Input: " << dataValue 
                  << " (gespeicherter Zustand = " << m_storedState << ")" << std::endl;
    } else {
        std::cout << "[CLOCK] " << m_name << " (EN=0) speichert NICHT (Zustand = " << m_storedState << ")" << std::endl;
    }
}

/**
 * Gibt den Zustand dieses D-Flip-Flops mit Enable aus
 */
void DFlipFlopEnable::printState() const {
    std::string d_pin = (m_inputs[0]) ? "verbunden" : "FLOATING";
    std::string en_pin = (m_inputs[1]) ? "verbunden" : "FLOATING";
    std::cout << "DFlipFlopEnable [" << m_name << ": D=" << d_pin 
              << ", EN=" << en_pin
              << ", Stored=" << (m_storedState ? 1 : 0)
              << "] => Output=" << (m_output ? 1 : 0) << std::endl;
}

/**
 * Virtueller Destruktor
 */
DFlipFlopEnable::~DFlipFlopEnable() {
    std::cout << "[DESTRUKTOR] Zerstöre D-Flip-Flop mit Enable: " << m_name << std::endl;
}
