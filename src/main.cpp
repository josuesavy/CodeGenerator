#include "Public.h"
#include "GlobalTranslator.h"

#include <iostream>
#include <chrono>

int main(int argc, char *argv[])
{   
    auto start = std::chrono::high_resolution_clock::now();

    GlobalTranslator globalTranslator1("C:/Users/SAVY/Documents/Dofus/Sources-Codes/" + std::string(MAJ_VERSION) + "/scripts",
                                       "C:/Users/SAVY/Documents/Translate",
                                       TRANSLATION_1);

    globalTranslator1.parse();
    globalTranslator1.serialize();
    globalTranslator1.write();

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Translation 1 completed in " << elapsed << "ms" << std::endl << std::endl;


    start = std::chrono::high_resolution_clock::now();

    GlobalTranslator globalTranslator2("C:/Users/SAVY/Documents/Dofus/Sources-Codes/" + std::string(MAJ_VERSION) + "/scripts",
                                       "C:/Users/SAVY/Documents/Translate",
                                       TRANSLATION_2);

    globalTranslator2.parse();
    globalTranslator2.serialize();
    globalTranslator2.write();

    end = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Translation 2 completed in " << elapsed << " ms" << std::endl << std::endl;

    return 0;
}
