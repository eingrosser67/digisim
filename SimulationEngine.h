#pragma once

#include <vector>
#include <memory>
#include "Component.h"

/**
 * SimulationEngine (Zusatzaufgabe 3)
 * 
 * Elegant verwaltete Bauteil-Kollektion für komplexe Schaltkreise
 * 
 * Statt mühsam in der main.cpp jedes Gatter einzeln reset() zu nennen,
 * verwendet man einfach:
 *   engine.addGate(myGate);
 *   ...
 *   engine.resetAll();
 * 
 * Das ist die typische Architektur in echten Chip-Simulatoren (SPICE, Vivado, etc.)
 */
class SimulationEngine {
private:
    std::vector<std::shared_ptr<Gate>> m_gates;  // Der "Bauteil-Kasten"

public:
    /**
     * Konstruktor: Leer, Gatter werden via addGate() hinzugefügt
     */
    SimulationEngine() = default;

    /**
     * Registriert ein Gatter/Component bei der Engine
     * @param gate Das hinzuzufügende Gatter (als shared_ptr)
     */
    void addGate(std::shared_ptr<Gate> gate) {
        m_gates.push_back(gate);
    }

    /**
     * Setzt bei ALLEN registrierten Gattern das Cache-Flag zurück
     * Dies ist der Knackpunkt für Phase 4: Ein Aufruf statt vieler!
     */
    void resetAll() {
        for (auto gate : m_gates) {
            gate->reset();
        }
    }

    /**
     * Gibt die Anzahl registrierter Gatter zurück (Debugging-Info)
     */
    size_t gateCount() const {
        return m_gates.size();
    }

    /**
     * Destruktor: Smart Pointers kümmern sich um Speicher
     */
    virtual ~SimulationEngine() = default;
};
