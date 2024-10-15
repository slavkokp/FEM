#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableView>
#include <QGridLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QDoubleValidator>
#include <QDoubleSpinBox>
#include <QTabWidget>
#include <QSpinBox>
#include <QGroupBox>
#include <QCheckbox>
#include "displaytablemodel.h"
#include "UsageInterface/FEMInterface.h"
#include "matplot/matplot.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void onDeleteRowsButtonClicked();
    void onAddRowsButtonClicked();
    void onSaveConfigurationClicked();
    void onEValueChanged(double val);
    void onNuValueChanged(double val);
    void onLoadAktActionClicked();
    void onLoadZpActionClicked();
    void onLoadZuActionClicked();
    void onSaveZuActionClicked();
    void onSaveZpActionClicked();
    void onSaveAktActionClicked();
    void onSaveMovementActionClicked();
    void onSaveTensionActionClicked();
    void onSaveUpdatedVerticesActionClicked();
    void onCalculateButtonClicked();
    void onPlotVerticesButtonClicked();
    void onPlotUpdatedVerticesButtonClicked();
    void onVerticesHelpClicked();
    void onVertexUpdated(const QModelIndex &begin, const QModelIndex &end);
    void onVertexSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    QPushButton* calculateButton;
    QPushButton* plotVerticesButton;
    QPushButton* plotUpdatedVerticesButton;
    QTableView* zpView;
    QTableView* zuView;
    QTableView* vertexView;
    QTableView* movementView;
    QTableView* tensionView;
    QTableView* updatedVerticesView;
    DisplayTableModel* zpTableModel;
    DisplayTableModel* zuTableModel;
    DisplayTableModel* vertexTableModel;
    DisplayTableModel* movementTableModel;
    DisplayTableModel* tensionsTableModel;
    DisplayTableModel* updatedVerticesTableModel;
    QGridLayout* layout;
    QPushButton* deleteRowsButton;
    QPushButton* addRowsButton;
    QTabWidget* tablesTabWidget;
    QGroupBox* quantityBox;
    QGroupBox* lengthsBox;
    QGroupBox* parametersBox;
    QCheckBox* showGlobalCarcassCheckBox;
    QCheckBox* enableVertexHighlightingCheckBox;

    // params inputs
    QDoubleSpinBox* eInput;
    QDoubleSpinBox* nuInput;
    QDoubleSpinBox* lambdaInput;
    QDoubleSpinBox* muInput;
    QVector<QDoubleSpinBox*> xyzLengths;
    QVector<QSpinBox*> xyzQuantity;

    enum class CurrentPlot
    {
        None = 0,
        Vertices,
        UpdatedVertices,
        Deprecated
    };

    fem::FEMInterface algoInterface;
    matplot::line_handle scatterHandle;
    CurrentPlot currPlot;
    bool currentHasCarcass;
    void showMessage(const QString& message);
    void showError(const QString& message);
    void initTableTabs();
    void initTableModels();
    void initTableViews();
    void initXyzQuantityGroupBox();
    void initXyzLengthsGroupBox();
    void initParamsGroupBox();
    void initMenuBar();
    void initButtons();
    void initVertexSelectionHighlight();
    QUrl showFileDialog(const QString& caption);

};
#endif // MAINWINDOW_H





