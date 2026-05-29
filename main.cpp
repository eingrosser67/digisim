#include <iostream>
#include <vector>
#include <memory>
#include "Component.h"
#include "AndGate.h"
#include "OrGate.h"
#include "XorGate.h"
#include "Switch.h"

/**
 * =====================================================================
 * Labor 7: Schaltkreisaufbau mit Smart Pointers (DAG-Architektur)
 * =====================================================================
 *
 * Diese Demonstration zeigt den Aufbau eines Volladdierers (Full Adder):
 * - Drei Eingangsbits: A, B, CarryIn
 * - Zwei Halbaddierer (jeweils XOR/AND)
 * - Ein OR-Gatter zur Ermittlung von CarryOut
 *
 * Volladdierer-Logik:
 *  HA1: Sum1 = A XOR B, Carry1 = A AND B
 *  HA2: Sum = Sum1 XOR CarryIn, Carry2 = Sum1 AND CarryIn
 *  CarryOut = Carry1 OR Carry2
 */

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  C++ Digital Simulator - Labor 7" << std::endl;
    std::cout << "  Volladdierer (Full Adder) Schaltkreis" << std::endl;
    std::cout << "========================================\n"
              << std::endl;

    std::cout << "[SCHRITT 1] Komponenten instanziieren (als Smart Pointers)...\n"
              << std::endl;

    // Datenquellen (Schalter)
    auto swA = std::make_shared<Switch>("Input A");
    auto swB = std::make_shared<Switch>("Input B");
    auto swCin = std::make_shared<Switch>("Carry In");

    // Halbaddierer 1
    auto xor1 = std::make_shared<XorGate>("XOR1 (HA1 Summe)");
    auto and1 = std::make_shared<AndGate>("AND1 (HA1 Carry)");

    // Halbaddierer 2
    auto xor2 = std::make_shared<XorGate>("XOR2 (HA2 Summe)");
    auto and2 = std::make_shared<AndGate>("AND2 (HA2 Carry)");

    // Finaler Carry-Out
    auto orGate = std::make_shared<OrGate>("OR (CarryOut)");

    // Alias-Namen für die finalen Ausgänge (lesbarer 'Root' der DAG)
    auto finalSumXor = xor2;    // finaler Sum-Ausgang
    auto finalCarryOr = orGate; // finaler Carry-Ausgang

    // Netzwerkliste: Alle Gatter sammeln, damit wir sie vor jedem Test zurücksetzen können
    std::vector<std::shared_ptr<Gate>> network;
    network.push_back(swA);
    network.push_back(swB);
    network.push_back(swCin);
    network.push_back(xor1);
    network.push_back(and1);
    network.push_back(xor2);
    network.push_back(and2);
    network.push_back(orGate);

    std::cout << "\n[SCHRITT 2] Schaltkreis verkabeln (DAG-Aufbau)...\n"
              << std::endl;

    // Halbaddierer 1: A, B
    xor1->connectInput(0, swA);
    xor1->connectInput(1, swB);
    and1->connectInput(0, swA);
    and1->connectInput(1, swB);

    // Halbaddierer 2: Sum1, CarryIn
    xor2->connectInput(0, xor1);
    xor2->connectInput(1, swCin);
    and2->connectInput(0, xor1);
    and2->connectInput(1, swCin);

    // Finaler Carry-Out: Carry1 OR Carry2
    orGate->connectInput(0, and1);
    orGate->connectInput(1, and2);

    std::cout << "\n[SCHRITT 3] Wahrheitstabelle: Alle 8 Kombinationen testen...\n"
              << std::endl;
    std::cout << "┌─────┬─────┬─────────┬─────────┐" << std::endl;
    std::cout << "│  A  │  B  │ CarryIn │ CarryOut│" << std::endl;
    std::cout << "├─────┼─────┼─────────┼─────────┤" << std::endl;

    std::vector<std::tuple<bool, bool, bool>> testCases = {
        {false, false, false},
        {false, false, true},
        {false, true, false},
        {false, true, true},
        {true, false, false},
        {true, false, true},
        {true, true, false},
        {true, true, true}};

    int testCount = 0;
    int passedCount = 0;

    for (auto [a, b, cin] : testCases)
    {
        // 1. Alle Caches / Vorberechnungen zurücksetzen
        for (auto &g : network)
            g->reset();

        // 2. Schalter neu setzen
        swA->setState(a);
        swB->setState(b);
        swCin->setState(cin);

        // 3. Einmalige Auswertung an den finalen Ausgängen (Pull-Prinzip)
        finalSumXor->evaluate();
        finalCarryOr->evaluate();

        bool sum = finalSumXor->getOutput();
        bool carryOut = finalCarryOr->getOutput();

        bool expectedSum = (a ^ b) ^ cin;
        bool expectedCarry = (a && b) || (cin && (a ^ b));

        std::cout << "│  " << (a ? 1 : 0) << "  │  " << (b ? 1 : 0) << "  │   "
                  << (cin ? 1 : 0) << "    │   " << (carryOut ? 1 : 0)
                  << "   │" << std::endl;

        testCount += 2;
        if (sum == expectedSum)
            passedCount++;
        if (carryOut == expectedCarry)
            passedCount++;
    }

    std::cout << "└─────┴─────┴─────────┴─────────┘" << std::endl;

    std::cout << "\n[SCHRITT 4] Zustandsbericht der Komponenten:\n"
              << std::endl;

    swA->printState();
    swB->printState();
    swCin->printState();
    xor1->printState();
    and1->printState();
    xor2->printState();
    and2->printState();
    orGate->printState();

    std::cout << "\n========================================" << std::endl;
    std::cout << "Test Summary:" << std::endl;
    std::cout << "Bestanden: " << passedCount << " / " << testCount << std::endl;
    std::cout << "========================================" << std::endl;

    if (passedCount == testCount)
    {
        std::cout << "\n[SUCCESS] Volladdierer funktioniert korrekt! ✓" << std::endl;
        return 0;
    }
    else
    {
        std::cerr << "\n[FEHLER] Mindestens ein Test fehlgeschlagen!" << std::endl;
        return 1;
    }
}
