#include "mainwindow.h"
#include <QMessageBox>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QFileDialog>
#include "UsageInterface/InputValidator.h"

#include "zpdatacreator.h"
#include "zudatacreator.h"
#include "vertexcreator.h"
#include "converter.h"
#include "plot_functions.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    calculateButton(new QPushButton("Calculate", this)),
    plotVerticesButton(new QPushButton("Plot vertices", this)),
    plotUpdatedVerticesButton(new QPushButton("Plot updated vertices", this)),
    zpView(new QTableView()),
    zuView(new QTableView()),
    vertexView(new QTableView()),
    movementView(new QTableView()),
    tensionView(new QTableView()),
    updatedVerticesView(new QTableView()),
    deleteRowsButton(new QPushButton("Delete selected rows", this)),
    addRowsButton(new QPushButton("Add new row", this)),
    tablesTabWidget(new QTabWidget(this)),
    quantityBox(new QGroupBox(tr("Quantity"), this)),
    lengthsBox(new QGroupBox(tr("Lengths"), this)),
    parametersBox(new QGroupBox(tr("Parameters"), this)),
    showGlobalCarcassCheckBox(new QCheckBox("Carcass", this)),
    enableVertexHighlightingCheckBox(new QCheckBox("Enable vertex highlighting", this)),
    eInput(new QDoubleSpinBox(this)),
    nuInput(new QDoubleSpinBox(this)),
    lambdaInput(new QDoubleSpinBox(this)),
    muInput(new QDoubleSpinBox(this)),
    currPlot(CurrentPlot::None),
    currentHasCarcass(false)
{
    //locale
    this->setLocale(QLocale::Ukrainian);


    this->setCentralWidget(new QWidget(this));
    this->layout = new QGridLayout(this->centralWidget());
    this->centralWidget()->setLayout(this->layout);

    // table models
    this->initTableModels();
    this->initTableViews();
    this->initTableTabs();

    this->initXyzQuantityGroupBox();
    this->initXyzLengthsGroupBox();


    this->initParamsGroupBox();

    this->initMenuBar();

    this->initButtons();
    this->showGlobalCarcassCheckBox->setCheckState(Qt::CheckState::Checked);
    this->enableVertexHighlightingCheckBox->setCheckState(Qt::CheckState::Checked);

    this->initVertexSelectionHighlight();

    // main window layout is grid
    QVBoxLayout* inputBoxesBox = new QVBoxLayout();
    inputBoxesBox->addWidget(parametersBox);
    inputBoxesBox->addWidget(quantityBox);
    inputBoxesBox->addWidget(lengthsBox);
    this->layout->addLayout(inputBoxesBox, 0, 1, 2, 1);
    this->layout->addWidget(this->tablesTabWidget, 0, 0, 2, 1);
    this->layout->addWidget(this->calculateButton, 4, 0);
    this->layout->addWidget(this->plotVerticesButton, 7, 0);
    this->layout->addWidget(this->plotUpdatedVerticesButton, 8, 0);
    this->layout->addWidget(this->deleteRowsButton, 3, 0);
    this->layout->addWidget(this->addRowsButton, 2, 0);
    this->layout->addWidget(this->showGlobalCarcassCheckBox, 5, 0);
    this->layout->addWidget(this->enableVertexHighlightingCheckBox, 6, 0);
    this->layout->setColumnMinimumWidth(0, 400);
    this->layout->setColumnMinimumWidth(1, 400);

    this->move(400, 300);
    connect(this->vertexTableModel, &QAbstractItemModel::dataChanged, this, &MainWindow::onVertexUpdated);
}

MainWindow::~MainWindow() {}

void MainWindow::initTableTabs()
{
    this->tablesTabWidget->addTab(this->zuView, this->zuView->objectName());
    this->tablesTabWidget->addTab(this->zpView, this->zpView->objectName());
    this->tablesTabWidget->addTab(this->vertexView, this->vertexView->objectName());
    this->tablesTabWidget->addTab(this->movementView, this->movementView->objectName());
    this->tablesTabWidget->addTab(this->tensionView, this->tensionView->objectName());
    this->tablesTabWidget->addTab(this->updatedVerticesView, this->updatedVerticesView->objectName());
}

void MainWindow::initTableModels()
{
    this->zpTableModel = new DisplayTableModel(this, new ZPDataCreator());
    this->zuTableModel = new DisplayTableModel(this, new ZUDataCreator());
    this->vertexTableModel = new DisplayTableModel(this, new VertexCreator());
    this->movementTableModel = new DisplayTableModel(this, new VertexCreator());
    this->tensionsTableModel = new DisplayTableModel(this, new VertexCreator());
    this->updatedVerticesTableModel = new DisplayTableModel(this, new VertexCreator());
}

void MainWindow::initTableViews()
{
    this->zpView->setModel(zpTableModel);
    this->zpView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->zuView->setModel(zuTableModel);
    this->zuView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->vertexView->setModel(vertexTableModel);
    this->vertexView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->movementView->setModel(movementTableModel);
    this->movementView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    this->movementView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->movementView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->tensionView->setModel(tensionsTableModel);
    this->tensionView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    this->tensionView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->tensionView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->updatedVerticesView->setModel(updatedVerticesTableModel);
    this->updatedVerticesView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    this->updatedVerticesView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->updatedVerticesView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->zuView->setObjectName("ZU");
    this->zpView->setObjectName("ZP");
    this->vertexView->setObjectName("Vertices");
    this->movementView->setObjectName("Movements");
    this->tensionView->setObjectName("Tensions");
    this->updatedVerticesView->setObjectName("Updated vertices");
}

void MainWindow::initXyzQuantityGroupBox()
{
    this->xyzQuantity = QVector<QSpinBox*>(3);
    for (int i = 0; i < 3; ++i)
    {
        this->xyzQuantity[i] = new QSpinBox();
        this->xyzQuantity[i]->setMinimum(1);
        this->xyzQuantity[i]->setMaximum(INT_MAX);
    }
    auto quantityForm = new QFormLayout();
    QString formLabels[] {"x", "y", "z"};
    for (int i = 0; i < 3; ++i)
    {
        quantityForm->addRow(formLabels[i], this->xyzQuantity[i]);
    }
    quantityBox->setLayout(quantityForm);
}

void MainWindow::initXyzLengthsGroupBox()
{
    this->xyzLengths = QVector<QDoubleSpinBox*>(3);
    for (int i = 0; i < 3; ++i)
    {
        this->xyzLengths[i] = new QDoubleSpinBox();
        this->xyzLengths[i]->setDecimals(3);
        this->xyzLengths[i]->setMinimum(0);
        this->xyzLengths[i]->setMaximum(DBL_MAX);
    }
    auto lengthsForm = new QFormLayout();
    QString formLabels[] {"x", "y", "z"};
    for (int i = 0; i < 3; ++i)
    {
        lengthsForm->addRow(formLabels[i], this->xyzLengths[i]);
    }
    lengthsBox->setLayout(lengthsForm);
}

void MainWindow::initParamsGroupBox()
{
    QFormLayout* paramsFormLayout = new QFormLayout();
    paramsFormLayout->addRow(tr("e"), eInput);
    paramsFormLayout->addRow(tr("nu"), nuInput);
    paramsFormLayout->addRow(tr("lambda (preview)"), lambdaInput);
    paramsFormLayout->addRow(tr("mu (preview)"), muInput);

    eInput->setDecimals(4);
    eInput->setMaximum(1000);
    eInput->setMinimum(0.0001);
    eInput->setValue(110);
    nuInput->setDecimals(4);
    nuInput->setMaximum(1000);
    nuInput->setMinimum(0.0001);
    nuInput->setValue(0.35);
    lambdaInput->setReadOnly(true);
    lambdaInput->setDecimals(4);
    lambdaInput->setMaximum(10000);
    lambdaInput->setMinimum(-10000);
    muInput->setReadOnly(true);
    muInput->setDecimals(4);
    muInput->setMaximum(10000);
    muInput->setMinimum(0.0001);
    this->onEValueChanged(eInput->value());
    this->onNuValueChanged(nuInput->value());
    this->parametersBox->setLayout(paramsFormLayout);

    connect(this->eInput, &QDoubleSpinBox::valueChanged, this, &MainWindow::onEValueChanged);
    connect(this->nuInput, &QDoubleSpinBox::valueChanged, this, &MainWindow::onNuValueChanged);
}

void MainWindow::initMenuBar()
{
    QMenu* handle = this->menuBar()->addMenu(tr("&Load"));
    QAction* loadZuAction = new QAction(tr("Load ZU from file"), this);
    QAction* loadZpAction = new QAction(tr("Load ZP from file"), this);
    QAction* loadAktAction = new QAction(tr("Load AKT from file"), this);
    handle->addAction(loadZuAction);
    handle->addAction(loadZpAction);
    handle->addAction(loadAktAction);

    handle = this->menuBar()->addMenu(tr("&Export"));
    QAction* saveZuAction = new QAction(tr("Save ZU to file"), this);
    QAction* saveZpAction = new QAction(tr("Save ZP to file"), this);
    QAction* saveAktAction = new QAction(tr("Save AKT to file"), this);
    QAction* saveMovementAction = new QAction(tr("Save movements to file"), this);
    QAction* saveTensionAction = new QAction(tr("Save tensions to file"), this);
    QAction* saveUpdatedVerticesAction = new QAction(tr("Save updated vertices to file"), this);
    handle->addAction(saveZuAction);
    handle->addAction(saveZpAction);
    handle->addAction(saveAktAction);
    handle->addAction(saveMovementAction);
    handle->addAction(saveTensionAction);
    handle->addAction(saveUpdatedVerticesAction);

    handle = this->menuBar()->addMenu(tr("&Help"));
    QAction* verticesFormatAction = new QAction(tr("Vertices format"), this);
    handle->addAction(verticesFormatAction);

    connect(loadAktAction, &QAction::triggered, this, &MainWindow::onLoadAktActionClicked);
    connect(loadZuAction, &QAction::triggered, this, &MainWindow::onLoadZuActionClicked);
    connect(loadZpAction, &QAction::triggered, this, &MainWindow::onLoadZpActionClicked);

    connect(saveZuAction, &QAction::triggered, this, &MainWindow::onSaveZuActionClicked);
    connect(saveZpAction, &QAction::triggered, this, &MainWindow::onSaveZpActionClicked);
    connect(saveAktAction, &QAction::triggered, this, &MainWindow::onSaveAktActionClicked);
    connect(saveMovementAction, &QAction::triggered, this, &MainWindow::onSaveMovementActionClicked);
    connect(saveTensionAction, &QAction::triggered, this, &MainWindow::onSaveTensionActionClicked);
    connect(saveUpdatedVerticesAction, &QAction::triggered, this, &MainWindow::onSaveUpdatedVerticesActionClicked);

    connect(verticesFormatAction, &QAction::triggered, this, &MainWindow::onVerticesHelpClicked);
}

void MainWindow::initButtons()
{
    connect(this->deleteRowsButton, &QPushButton::clicked, this, &MainWindow::onDeleteRowsButtonClicked);
    connect(this->calculateButton, &QPushButton::clicked, this, &MainWindow::onCalculateButtonClicked);
    connect(this->addRowsButton, &QPushButton::clicked, this, &MainWindow::onAddRowsButtonClicked);
    connect(this->plotVerticesButton, &QPushButton::clicked, this, &MainWindow::onPlotVerticesButtonClicked);
    connect(this->plotUpdatedVerticesButton, &QPushButton::clicked, this, &MainWindow::onPlotUpdatedVerticesButtonClicked);
}

void MainWindow::initVertexSelectionHighlight()
{
    connect(this->vertexView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onVertexSelectionChanged);
}

void MainWindow::onLoadZpActionClicked()
{
    QUrl location = showFileDialog("Choose ZP source");
    if(!location.isEmpty())
    {
        this->zpTableModel->loadZPS(location.path().removeAt(0));
    }
}

void MainWindow::onLoadZuActionClicked()
{
    QUrl location = showFileDialog("Choose ZU source");
    if(!location.isEmpty())
    {
        this->zuTableModel->loadZPS(location.path().removeAt(0));
    }
}

void MainWindow::onLoadAktActionClicked()
{
    QUrl location = showFileDialog("Choose global AKT source");
    if(!location.isEmpty())
    {
        this->vertexTableModel->loadZPS(location.path().removeAt(0));
    }
}

void MainWindow::onSaveZuActionClicked()
{
    this->zuTableModel->saveZPS("zu_qt.txt");
}

void MainWindow::onSaveZpActionClicked()
{
    this->zpTableModel->saveZPS("zp_qt.txt");
}

void MainWindow::onSaveAktActionClicked()
{
    this->vertexTableModel->saveZPS("akt_qt.txt");
}
void MainWindow::onSaveMovementActionClicked()
{
    this->movementTableModel->saveZPS("movements_qt.txt");
}

void MainWindow::onSaveTensionActionClicked()
{
    this->tensionsTableModel->saveZPS("tensions_qt.txt");
}

void MainWindow::onSaveUpdatedVerticesActionClicked()
{
    this->updatedVerticesTableModel->saveZPS("updated_vertices.txt");
}

void MainWindow::onDeleteRowsButtonClicked()
{
    QAbstractTableModel* tableModel;
    QItemSelectionModel *selectionModel;
    if(this->tablesTabWidget->currentWidget()->objectName() == "ZP")
    {
        tableModel = this->zpTableModel;
        selectionModel = this->zpView->selectionModel();
    }
    else if (this->tablesTabWidget->currentWidget()->objectName() == "ZU")
    {
        tableModel = this->zuTableModel;
        selectionModel = this->zuView->selectionModel();
    }
    else
    {
        return;
    }
    if(selectionModel->hasSelection())
    {
        QModelIndexList selectedRows = selectionModel->selectedRows();
        int size = selectedRows.size();
        QMessageBox warn;
        warn.setText(QString("Do you really want to delete %1 selected row(s)?").arg(QString::number(size)));
        warn.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes == warn.exec())
        {
            std::sort(selectedRows.begin(), selectedRows.end(),
                      [](const QModelIndex &a, const QModelIndex &b)
                        {
                            return a.row() > b.row();
                        });
            for (int i = 0; i < size; ++i)
            {
                tableModel->removeRow(selectedRows[i].row());
            }
        }
    }
}

void MainWindow::onCalculateButtonClicked()
{
    algoInterface.setE(this->eInput->value());
    algoInterface.setNu(this->nuInput->value());
    algoInterface.setZP(Converter::ZPDataQVecToStdVec(this->zpTableModel->zps));
    algoInterface.setZU(Converter::ZUDataQVecToStdVec(this->zuTableModel->zps));
    algoInterface.setGlobalAKT(Converter::aktVecToStdVec(this->vertexTableModel->zps));
    algoInterface.setXyzLengths(this->xyzLengths[0]->value(), this->xyzLengths[1]->value(), this->xyzLengths[2]->value());
    algoInterface.setXyzQuantity(this->xyzQuantity[0]->value(), this->xyzQuantity[1]->value(), this->xyzQuantity[2]->value());

    bool valid = fem::InputValidator::validateZPU(algoInterface.getZP(), algoInterface.getTotalFiniteElementsQuantity());
    if (!valid)
    {
        showMessage("Invalid ZP or quantities");
        return;
    }
    valid = fem::InputValidator::validateZPU(algoInterface.getZU(), algoInterface.getTotalFiniteElementsQuantity());
    if (!valid)
    {
        showMessage("Invalid ZU or quantities");
        return;
    }
    valid = algoInterface.getGlobalAKT()[0].size() == 20;
    if (!valid)
    {
        showMessage("Invalid vertices count. Must be 20.");
        return;
    }
    try
    {
        algoInterface.calculateAll();
        this->movementTableModel->setZPS(Converter::stdAktVecToQvec(algoInterface.getModifiedAKT()));
        this->tensionsTableModel->setZPS(Converter::stdTensionsVecToQvec(algoInterface.getTensions()));
        this->updatedVerticesTableModel->setZPS(Converter::stdAktVecToQvec(algoInterface.getModifiedAKT()));
        currPlot = CurrentPlot::Deprecated;
        this->showMessage("Done!");
    }
    catch(std::exception& exc)
    {
        QMessageBox::critical(this, tr("Calculation error"), tr(exc.what()));
    }
}

void MainWindow::onAddRowsButtonClicked()
{
    if(this->tablesTabWidget->currentWidget()->objectName() == "ZP")
    {
        this->zpTableModel->insertRow(zpTableModel->rowCount());
    }
    else if (this->tablesTabWidget->currentWidget()->objectName() == "ZU")
    {
        this->zuTableModel->insertRow(zuTableModel->rowCount());
    }
    else if (this->tablesTabWidget->currentWidget()->objectName() == "Vertices")
    {
        if(this->vertexTableModel->rowCount() < 20)
        {
            this->vertexTableModel->insertRow(vertexTableModel->rowCount());
        }
    }
}

void MainWindow::onPlotVerticesButtonClicked()
{
    if(this->vertexTableModel->rowCount() == 20)
    {
        if(this->currPlot != CurrentPlot::Vertices)
        {
            if (this->currPlot != CurrentPlot::None)
            {
                this->scatterHandle.reset();
                matplot::cla();
            }
            this->currPlot = CurrentPlot::Vertices;
            plot(Converter::globalAktToStdVec(this->vertexTableModel->zps), {1, 1, 1}, this->scatterHandle);
            setPointsColor("b", this->scatterHandle);
            setAxesLabels({"X", "Y", "Z"});

            setPlotWindowPosition(this->pos().x() + this->frameGeometry().width() + 320, this->pos().y() + 100);
            setPlotImageSize(this->frameGeometry().height(), this->frameGeometry().height() + 100);
            matplot::gcf()->draw();
        }
        else
        {
            if(this->scatterHandle.use_count() != 0)
            {
                matplot::gcf()->draw();
            }
        }
    }
}

void MainWindow::onPlotUpdatedVerticesButtonClicked()
{
    if(this->tensionsTableModel->rowCount() != 0)
    {
        if(this->currPlot != CurrentPlot::UpdatedVertices || currentHasCarcass != this->showGlobalCarcassCheckBox->isChecked())
        {
            if (this->currPlot != CurrentPlot::None)
            {
                this->scatterHandle.reset();
                matplot::cla();
            }
            this->currPlot = CurrentPlot::UpdatedVertices;
            plot(this->algoInterface.getModifiedAKT(), this->algoInterface.getXyzQuantity(), this->scatterHandle);
            currentHasCarcass = this->showGlobalCarcassCheckBox->isChecked();
            if(currentHasCarcass)
            {
                plot_carcass(Converter::changeOrderToDefault(this->algoInterface.getGlobalAKT()), {1, 1, 1}, "yellow");
            }
            setPointsColor(this->algoInterface.getTensions(), this->scatterHandle);
            setAxesLabels({"X", "Y", "Z"});

            setPlotWindowPosition(this->pos().x() + this->frameGeometry().width() + 320, this->pos().y() + 100);
            setPlotImageSize(this->frameGeometry().height(), this->frameGeometry().height() + 100);
            matplot::gcf()->draw();
        }
        else
        {
            if(this->scatterHandle.use_count() != 0)
            {
                matplot::gcf()->draw();
            }
        }
    }
}

void MainWindow::onVertexUpdated(const QModelIndex &begin, const QModelIndex &end)
{
    if(this->currPlot == CurrentPlot::Vertices)
    {
        this->currPlot = CurrentPlot::Deprecated;
        this->onPlotVerticesButtonClicked();
        this->currPlot = CurrentPlot::Vertices;
    }
}

void MainWindow::onSaveConfigurationClicked()
{

}

void MainWindow::onVerticesHelpClicked()
{
    this->showMessage("Vertices should include 20 points in order that corresponds to local numeration of nodes in standard finite element: " \
                        "0, 2, 7, 5, 12, 14, 19, 17, 1, 4, 6, 3, 8, 9, 11, 10, 13, 16, 18, 15");
}

void MainWindow::onEValueChanged(double e)
{
    double nu = this->nuInput->value();
    this->lambdaInput->setValue(e / ((1 + nu) * (1 - 2* nu)));
    this->muInput->setValue(e / (2 * (1 + nu)));
}

void MainWindow::onNuValueChanged(double nu)
{
    double e = this->eInput->value();
    this->lambdaInput->setValue(e / ((1 + nu) * (1 - 2* nu)));
    this->muInput->setValue(e / (2 * (1 + nu)));
}

void MainWindow::showMessage(const QString& message)
{
    QMessageBox warn;
    warn.setText(message);
    warn.exec();
}

void MainWindow::showError(const QString& message)
{

}

QUrl MainWindow::showFileDialog(const QString& caption)
{
    QUrl filePath = QFileDialog::getOpenFileUrl(this, caption, QUrl(), tr("Text File (*.txt);;Any files (*)"));
    return filePath;
}

void MainWindow::onVertexSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if(this->currPlot == CurrentPlot::Vertices && this->enableVertexHighlightingCheckBox->isChecked())
    {
        QModelIndexList selectedRows = this->vertexView->selectionModel()->selectedRows();
        int size = selectedRows.size();
        std::vector<double> colors(20, 0);
        for (int i = 0; i < size; ++i)
        {
            colors[Converter::order[selectedRows[i].row()]] = 1;
        }
        setPointsColor(colors, this->scatterHandle);
        matplot::gca()->draw();
    }
}
