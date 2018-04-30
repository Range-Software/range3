/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   value_set_generator_dialog.cpp                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   30-th May 2014                                           *
 *                                                                   *
 *  DESCRIPTION: Value set generator dialog class definition         *
 *********************************************************************/

#include <cmath>

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

#include "value_set_generator_dialog.h"

typedef enum _FuncType
{
    FUNC_FILL_A = 0,
    FUNC_LINEAR,
    FUNC_SIN,
    FUNC_COS,
    FUNC_N_FUNCS
} FuncType;

class ValueSetGeneratorStaticValues
{
    public:
        FuncType funcType;
        double a;
        double b;
        double c;
        double d;
    public:
        ValueSetGeneratorStaticValues()
        {
            this->initialize();
        }
        void initialize(void)
        {
            this->funcType = FUNC_SIN;
            this->a = 0.0;
            this->b = 1.0;
            this->c = 0.0;
            this->d = 1.0;
        }
};

QMap<QString,ValueSetGeneratorStaticValues> ValueSetGeneratorDialog::values = QMap<QString,ValueSetGeneratorStaticValues>();

static double funcFindValue(FuncType funcType, const ValueSetGeneratorStaticValues &values, double key)
{
    switch (funcType)
    {
        case FUNC_FILL_A:
        {
            return values.a;
        }
        case FUNC_LINEAR:
        {
            return values.a+values.b*key;
        }
        case FUNC_SIN:
        {
            return values.a+values.b+sin(RConstants::pi*(values.c+values.d*key));
        }
        case FUNC_COS:
        {
            return values.a+values.b+cos(RConstants::pi*(values.c+values.d*key));
        }
        default:
        {
            return 0.0;
        }
    }
}

static QString getFuncName(FuncType funcType, const QString &keySymbol)
{
    switch (funcType)
    {
        case FUNC_FILL_A:
        {
            return "A";
        }
        case FUNC_LINEAR:
        {
            return "A+B*" + keySymbol;
        }
        case FUNC_SIN:
        {
            return "A+B*sin(" + QString(QChar(0xC0, 0x03)) + "*(C+D*" + keySymbol + "))";
        }
        case FUNC_COS:
        {
            return "A+B*cos(" + QString(QChar(0xC0, 0x03)) + "*(C+D*" + keySymbol + "))";
        }
        default:
        {
            return QString();
        }
    }
}

ValueSetGeneratorDialog::ValueSetGeneratorDialog(RValueTable &valueTable, QWidget *parent)
    : QDialog(parent)
    , valueTable(valueTable)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    QString windowTitleStr = tr("Value set generator");
    this->setWindowTitle(tr(windowTitleStr.toUtf8().constData()));

    QGridLayout *mainLayout = new QGridLayout;
    this->setLayout (mainLayout);

    uint mainLayoutRowCnt = 0;

    QString key(this->valueTable.getKeyName() + "/" + this->valueTable.getValueName());
    bool isNew = !ValueSetGeneratorDialog::values.contains(key);
    ValueSetGeneratorStaticValues values = ValueSetGeneratorDialog::values[key];

    QLabel *valueLabel = new QLabel(this->valueTable.getValueName() + " = ");
    mainLayout->addWidget(valueLabel, mainLayoutRowCnt, 0, 1, 2);

    this->funcComboBox = new QComboBox;
    mainLayout->addWidget(this->funcComboBox, mainLayoutRowCnt++, 2, 1, 1);
    for (uint i=0;i<uint(FUNC_N_FUNCS);i++)
    {
        this->funcComboBox->addItem(getFuncName(FuncType(i),this->valueTable.getKeyName()));
    }

    this->connect(this->funcComboBox,
                  SIGNAL(currentIndexChanged(int)),
                  SLOT(onFuncComboCurrentIndexChanged(int)));

    QLabel *aLabel = new QLabel("A = ");
    mainLayout->addWidget(aLabel, mainLayoutRowCnt, 0, 1, 1);

    this->aLineEdit = new ValueLineEdit;
    mainLayout->addWidget(this->aLineEdit, mainLayoutRowCnt++, 1, 1, 2);
    this->aLineEdit->setValue(values.a);
    if (isNew && this->valueTable.size() > 0)
    {
        this->aLineEdit->setValue(this->valueTable.getValue(0));
    }

    QLabel *bLabel = new QLabel("B = ");
    mainLayout->addWidget(bLabel, mainLayoutRowCnt, 0, 1, 1);

    this->bLineEdit = new ValueLineEdit;
    mainLayout->addWidget(this->bLineEdit, mainLayoutRowCnt++, 1, 1, 2);
    this->bLineEdit->setValue(values.b);

    QLabel *cLabel = new QLabel("C = ");
    mainLayout->addWidget(cLabel, mainLayoutRowCnt, 0, 1, 1);

    this->cLineEdit = new ValueLineEdit;
    mainLayout->addWidget(this->cLineEdit, mainLayoutRowCnt++, 1, 1, 2);
    this->cLineEdit->setValue(values.c);

    QLabel *dLabel = new QLabel("D = ");
    mainLayout->addWidget(dLabel, mainLayoutRowCnt, 0, 1, 1);

    this->dLineEdit = new ValueLineEdit;
    mainLayout->addWidget(this->dLineEdit, mainLayoutRowCnt++, 1, 1, 2);
    this->dLineEdit->setValue(values.d);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout, mainLayoutRowCnt++, 0, 1, 3);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    this->okButton = new QPushButton(okIcon, tr("Ok"));
//    this->okButton->setEnabled(this->variableTree->selectedItems().size() && this->modelTree->selectedItems().size());
    okButton->setDefault(true);
    buttonsLayout->addWidget(this->okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&ValueSetGeneratorDialog::reject);
    QObject::connect(this->okButton,&QPushButton::clicked,this,&ValueSetGeneratorDialog::accept);

    this->funcComboBox->setCurrentIndex(values.funcType);
}

int ValueSetGeneratorDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        FuncType funcType = FuncType(this->funcComboBox->currentIndex());

        ValueSetGeneratorStaticValues values;

        values.funcType = FuncType(this->funcComboBox->currentIndex());
        values.a = this->aLineEdit->getValue();
        values.b = this->bLineEdit->getValue();
        values.c = this->cLineEdit->getValue();
        values.d = this->dLineEdit->getValue();
        for (uint i=0;i<this->valueTable.size();i++)
        {
            double key = this->valueTable.getKey(i);
            this->valueTable.add(key,funcFindValue(funcType,values,key));
        }

        ValueSetGeneratorDialog::values[this->valueTable.getKeyName() + "/" + this->valueTable.getValueName()] = values;
    }

    return retVal;
}

void ValueSetGeneratorDialog::onFuncComboCurrentIndexChanged(int index)
{
    this->aLineEdit->setEnabled(true);
    this->bLineEdit->setEnabled(true);
    this->cLineEdit->setEnabled(true);
    this->dLineEdit->setEnabled(true);

    switch (index)
    {
        case FUNC_FILL_A:
            this->bLineEdit->setDisabled(true);
            this->cLineEdit->setDisabled(true);
            this->dLineEdit->setDisabled(true);
            break;
        case FUNC_LINEAR:
            this->cLineEdit->setDisabled(true);
            this->dLineEdit->setDisabled(true);
            break;
        default:
            break;
    }
}
