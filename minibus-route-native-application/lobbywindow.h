#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class LobbyWindow; }
QT_END_NAMESPACE

class LobbyWindow : public QMainWindow
{
    Q_OBJECT

public:
    LobbyWindow(QWidget *parent = nullptr);
    ~LobbyWindow();

private:
    Ui::LobbyWindow *ui;
};
#endif // LOBBYWINDOW_H
