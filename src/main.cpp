#include "../lib/view.hpp"
#include "../lib/elements.hpp"

int main() {
    View* view = new View();
    view->loadBackground();
    view->displaying();
    delete view;
    return 0;
}