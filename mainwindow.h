#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
friend void initData();
friend void Astar();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void show_text(std::string);

    Ui::MainWindow *ui;

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


    void on_textBrowser_textChanged();
};
#endif // MAINWINDOW_H
