#pragma once
#include "Component.h"

class DFlipFlop : public Gate
{
private:
    bool m_storedState = false; // Interner Zustand des Flip-Flops (Q)

public:
    DFlipFlop(std::string n);

    void evaluate() override;
    void onClockTick();
    void printState() const override;
};
