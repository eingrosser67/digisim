#include <iostream>
#include <vector>
#include <memory>
#include "Component.h"
#include "AndGate.h"
#include "OrGate.h"
#include "NotGate.h"
#include "XorGate.h"
#include "NandGate.h"
#include "LogicEngine.h"

/**
 * =====================================================================
 * Labor 7: Automatisierte Tests mit Exit-Codes für CI/CD
 * =====================================================================
 * 
 * Diese Wahrheitstabellen-Tests verifizieren alle Gatter-Implementierungen.
 * Bei Fehler: Exit-Code 1 (FEHLER für die CI-Pipeline)
 * Bei Erfolg: Exit-Code 0 (GRÜNER HAKEN)
 */

// Hilfsfunktion: Testet ein Gatter mit gegebener Eingabe und erwartetem Output
bool testGate(Component* gate, bool inputA, bool inputB, bool expectedOutput, 
              const std::string& gateName) {
    gate->setInputA(inputA);
    gate->setInputB(inputB);
    gate->evaluate();  // ← KRITISCH: Berechne die Logik!
    bool actualOutput = gate->getOutput();
    
    bool testPassed = (actualOutput == expectedOutput);
    
    std::cout << "  " << gateName << ": A=" << inputA << " B=" << inputB 
              << " => " << actualOutput;
    if (testPassed) {
        std::cout << " ✓";
    } else {
        std::cout << " ✗ (erwartet: " << expectedOutput << ")";
    }
    std::cout << std::endl;
    
    return testPassed;
}

// Hilfsfunktion: Testet ein Gatter mit 1-Eingang (NOT-Gatter)
bool testGateNOT(Component* gate, bool input, bool expectedOutput, 
                 const std::string& gateName) {
    gate->setInputA(input);
    gate->setInputB(false);  // NOT-Gatter ignoriert InputB
    gate->evaluate();  // ← KRITISCH: Berechne die Logik!
    bool actualOutput = gate->getOutput();
    
    bool testPassed = (actualOutput == expectedOutput);
    
    std::cout << "  " << gateName << ": A=" << input 
              << " => " << actualOutput;
    if (testPassed) {
        std::cout << " ✓";
    } else {
        std::cout << " ✗ (erwartet: " << expectedOutput << ")";
    }
    std::cout << std::endl;
    
    return testPassed;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "C++ Digital Simulator - Automated Tests" << std::endl;
    std::cout << "Labor 7: GitHub Actions CI/CD Pipeline" << std::endl;
    std::cout << "========================================\n" << std::endl;

    bool allTestsPassed = true;
    int testCount = 0;
    int passedCount = 0;

    // =====================================================================
    // TEST 1: AND-Gate (Wahrheitstabelle)
    // =====================================================================
    std::cout << "[TEST 1] AND-Gate Wahrheitstabelle:" << std::endl;
    {
        auto andGate = std::make_unique<AndGate>("TEST-AND");
        
        std::vector<std::tuple<bool, bool, bool>> testCases = {
            {false, false, false},  // 0 & 0 = 0
            {false, true, false},   // 0 & 1 = 0
            {true, false, false},   // 1 & 0 = 0
            {true, true, true}      // 1 & 1 = 1
        };
        
        for (auto& [a, b, expected] : testCases) {
            testCount++;
            if (testGate(andGate.get(), a, b, expected, "AND")) {
                passedCount++;
            } else {
                allTestsPassed = false;
            }
        }
    }
    std::cout << std::endl;

    // =====================================================================
    // TEST 2: OR-Gate (Wahrheitstabelle)
    // =====================================================================
    std::cout << "[TEST 2] OR-Gate Wahrheitstabelle:" << std::endl;
    {
        auto orGate = std::make_unique<OrGate>("TEST-OR");
        
        std::vector<std::tuple<bool, bool, bool>> testCases = {
            {false, false, false},  // 0 | 0 = 0
            {false, true, true},    // 0 | 1 = 1
            {true, false, true},    // 1 | 0 = 1
            {true, true, true}      // 1 | 1 = 1
        };
        
        for (auto& [a, b, expected] : testCases) {
            testCount++;
            if (testGate(orGate.get(), a, b, expected, "OR")) {
                passedCount++;
            } else {
                allTestsPassed = false;
            }
        }
    }
    std::cout << std::endl;

    // =====================================================================
    // TEST 3: NOT-Gate (Wahrheitstabelle)
    // =====================================================================
    std::cout << "[TEST 3] NOT-Gate Wahrheitstabelle:" << std::endl;
    {
        auto notGate = std::make_unique<NotGate>("TEST-NOT");
        
        std::vector<std::tuple<bool, bool>> testCases = {
            {false, true},   // NOT 0 = 1
            {true, false}    // NOT 1 = 0
        };
        
        for (auto& [input, expected] : testCases) {
            testCount++;
            if (testGateNOT(notGate.get(), input, expected, "NOT")) {
                passedCount++;
            } else {
                allTestsPassed = false;
            }
        }
    }
    std::cout << std::endl;

    // =====================================================================
    // TEST 4: XOR-Gate (Wahrheitstabelle)
    // =====================================================================
    std::cout << "[TEST 4] XOR-Gate Wahrheitstabelle:" << std::endl;
    {
        auto xorGate = std::make_unique<XorGate>("TEST-XOR");
        
        std::vector<std::tuple<bool, bool, bool>> testCases = {
            {false, false, false},  // 0 XOR 0 = 0
            {false, true, true},    // 0 XOR 1 = 1
            {true, false, true},    // 1 XOR 0 = 1
            {true, true, false}     // 1 XOR 1 = 0
        };
        
        for (auto& [a, b, expected] : testCases) {
            testCount++;
            if (testGate(xorGate.get(), a, b, expected, "XOR")) {
                passedCount++;
            } else {
                allTestsPassed = false;
            }
        }
    }
    std::cout << std::endl;

    // =====================================================================
    // TEST 5: NAND-Gate (Wahrheitstabelle)
    // =====================================================================
    std::cout << "[TEST 5] NAND-Gate Wahrheitstabelle:" << std::endl;
    {
        auto nandGate = std::make_unique<NandGate>("TEST-NAND");
        
        std::vector<std::tuple<bool, bool, bool>> testCases = {
            {false, false, true},   // NAND(0,0) = 1
            {false, true, true},    // NAND(0,1) = 1
            {true, false, true},    // NAND(1,0) = 1
            {true, true, false}     // NAND(1,1) = 0
        };
        
        for (auto& [a, b, expected] : testCases) {
            testCount++;
            if (testGate(nandGate.get(), a, b, expected, "NAND")) {
                passedCount++;
            } else {
                allTestsPassed = false;
            }
        }
    }
    std::cout << std::endl;

    // =====================================================================
    // TEST 6: LogicEngine-Integration
    // =====================================================================
    std::cout << "[TEST 6] LogicEngine-Integration:" << std::endl;
    {
        LogicEngine engine;
        engine.setCircuitName("Integrations-Test");
        
        auto g1 = std::make_unique<AndGate>("AND-1");
        g1->setInputA(true);
        g1->setInputB(true);
        
        engine.addComponent(std::move(g1));
        
        std::cout << "  Engine enthält " << engine.getComponentCount() << " Komponente(n)" << std::endl;
        
        testCount++;
        if (engine.getComponentCount() == 1) {
            std::cout << "  Integration erfolgreich ✓" << std::endl;
            passedCount++;
        } else {
            std::cout << "  Integration fehlgeschlagen ✗" << std::endl;
            allTestsPassed = false;
        }
    }
    std::cout << std::endl;

    // =====================================================================
    // Abschluss und Exit-Code
    // =====================================================================
    std::cout << "========================================" << std::endl;
    std::cout << "Test Summary:" << std::endl;
    std::cout << "Bestanden: " << passedCount << " / " << testCount << std::endl;
    std::cout << "========================================" << std::endl;

    if (!allTestsPassed) {
        std::cerr << "\n[FEHLER] Mindestens ein Test fehlgeschlagen!" << std::endl;
        std::cerr << "Die CI-Pipeline wird dies als FEHLER markieren." << std::endl;
        return 1;  // ← EXIT-CODE 1: FEHLER (Rotes Kreuz für CI)
    }

    std::cout << "\n[SUCCESS] Alle Tests bestanden! ✓" << std::endl;
    std::cout << "Die CI-Pipeline wird dies als ERFOLG markieren." << std::endl;
    return 0;  // ← EXIT-CODE 0: ERFOLG (Grüner Haken für CI)
}