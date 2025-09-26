#include "FullScreen/FullScreen.h"
#include "Model/Model.h"
#ifdef slots
#undef slots
#endif
#ifdef signals
#undef signals
#endif
#ifdef emit
#undef emit
#endif
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    FullWindow window;
    window.show();

//    Model model("/media/kiri4800/LOLKEK/cpp/yolo_models/nano_6_07_08.pt");
//    model.test("/media/kiri4800/LOLKEK/cpp/test_reference.png");
    return app.exec();
}
