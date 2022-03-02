#ifndef FM_PRINCIPAL_H
#define FM_PRINCIPAL_H

#include <QMainWindow>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class Fm_principal; }
QT_END_NAMESPACE

class Fm_principal : public QMainWindow
{
    Q_OBJECT

public:
    Fm_principal(QWidget *parent = nullptr);
    ~Fm_principal();
    QIcon cadFechado;
    QIcon *cadAberto = new QIcon();

private slots:
    void on_btn_bloquear_clicked();
    void on_pushButton_clicked();
    void on_actionEstoque_triggered();
    void on_actionColaboradores_triggered();
    void on_actionVendas_triggered();

private:
    Ui::Fm_principal *ui;
};
#endif // FM_PRINCIPAL_H
