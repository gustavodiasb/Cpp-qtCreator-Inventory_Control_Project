#ifndef FM_LOGAR_H
#define FM_LOGAR_H

#include <QDialog>
#include "conexao.h"

namespace Ui {
class Fm_logar;
}

class Fm_logar : public QDialog
{
    Q_OBJECT

public:
    explicit Fm_logar(QWidget *parent = nullptr);
    ~Fm_logar();
    bool logado;
    Conexao con;
    QString nome, acesso;
    int id;

private slots:
    void on_btn_login_clicked();
    void on_btn_cancel_clicked();

private:
    Ui::Fm_logar *ui;
};

#endif // FM_LOGAR_H
