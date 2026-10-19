#include "app/Menu.h"
#include "algorithms/RandomTSP.h"

int main() {
    initialize_random_seed();
    Menu menu;
    menu.run();  // Wywołanie menu
    return 0;
}
