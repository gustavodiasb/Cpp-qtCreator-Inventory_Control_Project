#ifndef FM_NOVAVENDA_H
#define FM_NOVAVENDA_H

#include <QDialog>
#include "conexao.h"
#include <QTableWidget>

namespace Ui {
class fm_novaVenda;
}

class fm_novaVenda : public QDialog
{
    Q_OBJECT

public:
    explicit fm_novaVenda(QWidget *parent = nullptr);
    ~fm_novaVenda();

    Conexao con;
    int numLinha;
    void limparCampos();
    double calculaTotal(QTableWidget *tw, int coluna);
    static QString g_idProduto, g_produto, g_quantidade, g_valorUnitario, g_valorTotal;
    static bool alteracao;

private slots:
    void on_txt_idProduto_returnPressed();

    void on_btn_excluir_clicked();
    void on_btn_editar_clicked();
    void on_btn_venda_clicked();

private:
    Ui::fm_novaVenda *ui;
};

#endif // FM_NOVAVENDA_H
