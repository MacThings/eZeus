#include "ehunter.h"

eHunter::eHunter(eGameBoard& board) :
    eResourceCollector(board, &eCharacterTextures::fHunter) {}
