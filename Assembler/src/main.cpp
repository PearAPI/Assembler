#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "AST/visitors/staticAnalisysVisitor.h"
#include "AST/visitors/symbolTableVisitor.h"
#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Parser/Parser.h"

namespace fs = std::filesystem;

std::vector<std::string> listFilesInDirectory(const std::string& directoryPath) {
    std::vector<std::string> filePaths;

    try {
        // Prüfen, ob der Pfad existiert und ein Verzeichnis ist
        if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
            std::cerr << "Fehler: Verzeichnis existiert nicht oder ist ungueltig: " << directoryPath << '\n';
            return filePaths;
        }

        // Über das Verzeichnis iterieren
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                // Wir speichern den kompletten Pfad zur Datei
                filePaths.push_back(entry.path().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem Fehler: " << e.what() << '\n';
    }

    return filePaths;
}

std::string readFileToString(const std::string& filePath) {
    // Datei im Lesemodus öffnen
    std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);

    if (!fileStream) {
        throw std::runtime_error("Konnte Datei nicht oeffnen: " + filePath);
    }

    // Inhalt über einen Stringstream in einen std::string laden
    std::ostringstream stringStream;
    stringStream << fileStream.rdbuf();

    return stringStream.str();
}

int main(int argc, char* argv[]) {
    std::string sourceCode =
        "main:\n"
        "mov ax, bx\n"
        "mov ax, 5\n"
        "loop:\n"
        "add bx, 100h\n"
        "mov ax, bh\n"
        "cmp ax, 800h\n"
        "je done\n"
        "jmp loop\n"
        "done:\n"
        "jmp main\n";

    std::cout << "[1/3] Lexer läuft...\n";
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();
    std::cout << "      -> " << tokens.size() << " Tokens erfolgreich generiert.\n\n";

    std::cout << "[2/3] Parser baut den AST...\n";
    Parser parser(tokens);
    std::unique_ptr<ProgramNode> astRoot;

    try {
        astRoot = parser.parse();
        std::cout << "      -> Abstract Syntax Tree erfolgreich aufgebaut.\n\n";
    } catch (const std::exception& e) {
        std::cerr << "      -> PARSER ABBRUCH: " << e.what() << "\n";
        return 1;  // Harter Abbruch bei Syntax-Fehlern
    }

    std::cout << "[3/4] Statische Analyse prüft die Logik...\n";
    StaticAnalysisVisitor analyzer;
    astRoot->accept(analyzer);

    if (analyzer.hasErrors()) {
        std::cerr << "\nFEHLER GEFUNDEN:\n";
        std::cerr << "----------------\n";
        analyzer.printErrors();
        std::cerr << "\nKompilierung fehlgeschlagen.\n";
        return 1;
    }

    std::cout << "[4/4] Erstelle Symboltabelle (Adressen berechnen)...\n";
    SymbolTableVisitor symbolVisitor;
    astRoot->accept(symbolVisitor);

    if (symbolVisitor.hasErrors()) {
        std::cerr << "\nSYMBOL FEHLER GEFUNDEN:\n";
        std::cerr << "-----------------------\n";
        symbolVisitor.printErrors();
        std::cerr << "\nKompilierung fehlgeschlagen.\n";
        return 1;
    }

    std::cout << "      -> Labels erfolgreich zugewiesen.\n\n";

    std::cout << "Label list: \n";
    for (const auto& label : symbolVisitor.table.labels) {
        std::cout << label.first << " -> " << label.second << "\n";
    }

    std::cout << "\nSUCCESS! Der Code ist syntaktisch und logisch einwandfrei.\n";

    return 0;
}