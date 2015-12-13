#include <QApplication>
#include <QPushButton>

using namespace std;

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QWidget window;
	window.setFixedSize(100, 50);

	QPushButton button("Hello World!", &window);
	button.setGeometry(10, 10, 80, 30);

	button.setText("My text");
	button.setToolTip("A tooltip");

	window.show();

	return app.exec();
}
