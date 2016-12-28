#include "form.h"
#include "ui_form.h"
#include "../cache.h"
#include <QtCore/QObject>
#include <QtWidgets/QPushbutton.h>
#include <QtWidgets/QDoubleSpinBox>

Form::Form() :
    QWidget(nullptr),
    m_ui(std::make_unique<Ui::Form>())
{
    m_ui->setupUi(this);
}

Form::~Form()
{
}

Form::Form(std::shared_ptr<Cache> cache) :
    QWidget(nullptr),
    m_ui(std::make_unique<Ui::Form>()),
    m_cache(cache)
{
    m_ui->setupUi(this);
    m_ui->testSpinBox->setValue(0.0);

    connect(m_ui->testButton, SIGNAL(released(void)),
        this, SLOT(OnExampleButtonRelease(void)));

    connect(m_ui->testSpinBox, SIGNAL(valueChanged(double)),
        this, SLOT(OnExampleSpinBoxChange(double)));
}

void Form::OnExampleButtonRelease()
{
    m_cache->ExampleButton.Set(true);
}

void Form::OnExampleSpinBoxChange(double value)
{
    m_cache->ExampleSpinBox.Set(value);
}
