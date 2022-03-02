#ifndef FM_GESTAOCOLABORADORES_H
#define FM_GESTAOCOLABORADORES_H

#include <QDialog>
#include "conexao.h"

namespace Ui {
class fm_gestaoColaboradores;
}

class fm_gestaoColaboradores : public QDialog
{
    Q_OBJECT

public:
    explicit fm_gestaoColaboradores(QWidget *parent = nullptr);
    ~fm_gestaoColaboradores();

    Conexao con;

    void limparCampos();

private slots:
    void on_btn_cancelar_novoColab_clicked();
    void on_btn_novo_novoColab_clicked();
    void on_btn_gravar_novoColab_clicked();

    void on_btn_ge_cancelar_clicked();
    void on_btn_ge_novo_clicked();
    void on_btn_ge_gravar_clicked();
    void on_tabWidget_currentChanged(int index);

    void on_btn_ge_filtrar_clicked();

    void on_tw_ge_listarColab_itemSelectionChanged();

    void on_pushButton_clicked();

private:
    Ui::fm_gestaoColaboradores *ui;
};

#endif // FM_GESTAOCOLABORADORES_H
