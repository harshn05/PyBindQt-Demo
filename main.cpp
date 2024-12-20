#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <filesystem>
#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

namespace py = pybind11;
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Initialize Python interpreter
    py::scoped_interpreter guard{};
    // Determine the path to the extracted Python library
    fs::path exePath = fs::current_path(); // Adjust if needed
    fs::path pythonLibDir = exePath / "python312";

    // Set the Python path to include the directory
    py::module sys = py::module::import("sys");
    py::object path = sys.attr("path");
    path.attr("insert")(0, pythonLibDir.string());

    // Example Python code execution
    try
    {
        py::exec(R"(
            import sys
            print("Python path:", sys.path)
        )");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Create the main window
    QMainWindow mainWindow;
    QWidget centralWidget;
    QVBoxLayout layout(&centralWidget);

    // Create a QTextEdit to act as a Python console
    QTextEdit *pythonConsole = new QTextEdit();
    pythonConsole->setPlaceholderText("Python Console");
    pythonConsole->setReadOnly(true);
    layout.addWidget(pythonConsole);

    // Create a button to run Python code
    QPushButton *runButton = new QPushButton("Run Python Code");
    layout.addWidget(runButton);

    // Connect button click to a slot that runs Python code
    QObject::connect(runButton, &QPushButton::clicked, [=]()
                     {
        // Simple example of running a Python command
        std::string code = "print('Hello from Python!')";
        pythonConsole->append(QString::fromStdString(code));
        py::exec(code); });

    // Set the central widget and show the main window
    mainWindow.setCentralWidget(&centralWidget);
    mainWindow.show();

    return app.exec();
}
