#ifndef FM_GESTAOVENDAS_H
#define FM_GESTAOVENDAS_H

#include <QDialog>
#include "conexao.h"

namespace Ui {
class fm_gestaoVendas;
}

class fm_gestaoVendas : public QDialog
{
    Q_OBJECT

public:
    explicit fm_gestaoVendas(QWidget *parent = nullptr);
    ~fm_gestaoVendas();

    Conexao con;

    void definirDadosTabelaVenda(QString tipoBusca);

private slots:
    void on_tw_listarVenda_itemSelectionChanged();

    void on_btn_filtrarData_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::fm_gestaoVendas *ui;
};

#endif // FM_GESTAOVENDAS_H
