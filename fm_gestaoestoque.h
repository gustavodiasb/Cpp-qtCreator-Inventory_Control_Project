#ifndef FM_GESTAOESTOQUE_H
#define FM_GESTAOESTOQUE_H

#include <QDialog>
#include "conexao.h"

namespace Ui {
class fm_gestaoEstoque;
}

class fm_gestaoEstoque : public QDialog
{
    Q_OBJECT

public:
    explicit fm_gestaoEstoque(QWidget *parent = nullptr);
    ~fm_gestaoEstoque();
    Conexao con;

    void limparCampos();

private slots:
    void on_btn_novoProduto_clicked();
    void on_btn_gravarNovoProduto_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_tw_ge_produtos_itemSelectionChanged();

    void on_btn_ge_gravar_clicked();
    void on_btn_ge_excluir_clicked();
    void on_btn_ge_filtrar_clicked();

private:
    Ui::fm_gestaoEstoque *ui;
};

#endif // FM_GESTAOESTOQUE_H
