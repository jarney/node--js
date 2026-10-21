#include <node--js/gui/BreadcrumbsWidget.hpp>

using namespace NodeJS::gui;

BreadcrumbsWidget::BreadcrumbsWidget()
{
    layout = std::make_unique<QVBoxLayout>(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    scroll_area = new QScrollArea();
    scroll_area->setWidgetResizable(true);
    scroll_area->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    QWidget *inside = new QWidget();
    scroll_area->setWidget(inside);
    
    layout->addWidget(scroll_area);

    hbox_layout = new QHBoxLayout(inside);
    hbox_layout->setAlignment(Qt::AlignLeft);
    hbox_layout->setContentsMargins(0, 0, 0, 0);
    hbox_layout->setSpacing(0);
}

QWidget *
BreadcrumbsWidget::getPage()
{
    return pages.at(0);
}

void
BreadcrumbsWidget::addPage(QWidget *page)
{
    // Add the page
    if (pages.size() > 0) {
	QWidget *old = pages.at(pages.size()-1);
	old->hide();
	layout->removeWidget(old);

	// Add the button to select that page.
	QPushButton *button = new QPushButton();
	button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	std::string buttonName = std::string("Page ") + std::to_string(buttons.size()); 
	button->setText(QString::fromStdString(buttonName));

	int index = buttons.size();
	QObject::connect(button, &QPushButton::clicked, [this, index]() {
	    QSize buttonSize = buttons.at(buttons.size()-1)->size();
	    this->removePage(index);
	});
    
	buttons.push_back(button);
	hbox_layout->addWidget(button);
    }
    pages.push_back(page);
    layout->addWidget(page);
    layout->update();


}

void
BreadcrumbsWidget::removePage(int index)
{
    // Remove the current (last) page as long as it's not also the first.
    int current_page = pages.size()-1;

    // We should never end up with an empty list.
    if (current_page <= 0) return;
    // If the current page is already smaller than index,
    // there is nothing to do.
    if (current_page <= index) return;

    bool removed = false;
    
    while (buttons.size() > index) {
	QWidget *page = pages.at(pages.size()-1);
	if (!removed) {
	    page->hide();
	    layout->removeWidget(page);
	    removed = true;
	}
	delete page;
	pages.pop_back();
	
	QWidget *button = buttons.at(buttons.size()-1);
	hbox_layout->removeWidget(button);
	button->hide();
	delete button;
	buttons.pop_back();
    }
    
    // Add back the index of the page
    // we want to end up on.
    if (removed) {
	QWidget *page = pages.at(index);
	page->show();
	layout->addWidget(page);
    }
}

BreadcrumbsWidget::~BreadcrumbsWidget()
{
}
