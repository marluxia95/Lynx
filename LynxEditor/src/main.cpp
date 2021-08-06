#include <QApplication>
#include <QPushButton>

/*
    The editor is nothing for now, im working on it, this is just an example Qt code to see if its well configured/set up
*/

int main(int argc, char **argv)
{
 QApplication app (argc, argv);

 QPushButton button ("Test");
 button.show();

 return app.exec();
}