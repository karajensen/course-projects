#ifndef FORM_H
#define FORM_H

#include <QtWidgets/QWidget>
#include <memory>

struct Cache;

namespace Ui
{
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:

    /**
    * Constructor for the Qt application
    */
    Form();

    /**
    * Constructor for the VS application
    * @param cache Shared cache with the application
    */
    Form(std::shared_ptr<Cache> cache);

    /**
    * Destructor
    */
    ~Form();

private slots:

    /**
    * Called on release of the example button
    */
    void OnExampleButtonRelease();

    /**
    * Called on change of value of the spin box
    */
    void OnExampleSpinBoxChange(double value);

private:

    std::shared_ptr<Cache> m_cache; ///< Shared cache with the application
    std::unique_ptr<Ui::Form> m_ui; ///< The user interface
};

#endif // FORM_H
