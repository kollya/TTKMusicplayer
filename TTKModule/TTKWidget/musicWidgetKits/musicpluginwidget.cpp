#include "musicpluginwidget.h"
#include "ui_musicpluginwidget.h"
#include "musicuiobject.h"
#include "musicitemdelegate.h"
///qmmp incldue
#include "decoderfactory.h"
#include "outputfactory.h"
#include "visualfactory.h"
#include "effectfactory.h"
///qmmp incldue
#include "decoder.h"
#include "effect.h"
#include "output.h"
#include "visual.h"

#include <QScrollBar>

class MusicPluginItem : public QTreeWidgetItem
{
public:
    MusicPluginItem(QTreeWidgetItem *parent, DecoderFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, DECODER)
    {
        initialize(Decoder::isEnabled(factory), true, factory->properties().name, path);
        m_factory = factory;
    }

    MusicPluginItem(QTreeWidgetItem *parent, EffectFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, EFFECT)
    {

        initialize(Effect::isEnabled(factory), false, factory->properties().name, path);
        m_factory = factory;
    }

    MusicPluginItem(QTreeWidgetItem *parent, VisualFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, VISUAL)
    {
        initialize(Visual::isEnabled(factory), false, factory->properties().name, path);
        m_factory = factory;
    }

    MusicPluginItem(QTreeWidgetItem *parent, OutputFactory *factory, const QString &path)
        : QTreeWidgetItem(parent, OUTPUT)
    {
        initialize(Output::currentFactory() == factory, false, factory->properties().name, path);
        m_factory = factory;
    }

    enum PluginType
    {
        DECODER = QTreeWidgetItem::UserType,
        EFFECT,
        VISUAL,
        OUTPUT
    };

    void setEnabled(bool enabled)
    {
        switch(type())
        {
            case MusicPluginItem::DECODER:
                Decoder::setEnabled(TTKStatic_cast(DecoderFactory*, m_factory), enabled);
                break;
            case MusicPluginItem::EFFECT: break;
            case MusicPluginItem::VISUAL: break;
            case MusicPluginItem::OUTPUT: break;
            default: break;
        }
    }

    void initialize(bool state, bool enable, const QString &name, const QString &path)
    {
        setData(0, MUSIC_CHECK_ROLE, state ? Qt::Checked : Qt::Unchecked);
        setData(0, MUSIC_ENABL_ROLE, enable);
        setData(0, MUSIC_TEXTS_ROLE, name);
        setData(1, MUSIC_TEXTS_ROLE, path.section('/', -1));
    }

private:
    void *m_factory;
};



MusicPluginWidget::MusicPluginWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicPluginWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));

    m_ui->treeWidget->header()->setSectionsMovable(false);
    m_ui->treeWidget->header()->setMinimumSectionSize(250);
    m_ui->treeWidget->setHeaderLabels(QStringList() << tr("Description") << tr("Name"));

    MusicCheckBoxDelegate *checkDelegate = new MusicCheckBoxDelegate(this);
    checkDelegate->showTextMode(true);
    checkDelegate->setTreeModel(true);
    m_ui->treeWidget->setItemDelegateForColumn(0, checkDelegate);
    MusicLabelDelegate *labelDelegate = new MusicLabelDelegate(this);
    labelDelegate->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelDelegate->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->treeWidget->setItemDelegateForColumn(1, labelDelegate);

    m_ui->treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->treeWidget->verticalScrollBar()->setStyleSheet(MusicUIObject::MQSSScrollBarStyle03);

    loadPluginsInfo();

    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
    connect(m_ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(pluginItemChanged(QTreeWidgetItem*,int)));
}

MusicPluginWidget::~MusicPluginWidget()
{

}

void MusicPluginWidget::pluginItemChanged(QTreeWidgetItem *item, int column)
{
    if(column == 0 && item->type() == MusicPluginItem::DECODER)
    {
        const Qt::CheckState status = item->data(column, MUSIC_CHECK_ROLE).value<Qt::CheckState>();
        item->setData(column, MUSIC_CHECK_ROLE, status == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        TTKDynamic_cast(MusicPluginItem*, item)->setEnabled(status != Qt::Checked);
    }
}

int MusicPluginWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}

void MusicPluginWidget::loadPluginsInfo()
{
    m_ui->treeWidget->blockSignals(true);

    QTreeWidgetItem *item = nullptr;
    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Decoders"));
    item->setFirstColumnSpanned(true);
    foreach(DecoderFactory *factory, Decoder::factories())
    {
        new MusicPluginItem(item, factory,  Decoder::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Effects"));
    item->setFirstColumnSpanned(true);
    foreach(EffectFactory *factory, Effect::factories())
    {
        new MusicPluginItem(item, factory, Effect::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Visualization"));
    item->setFirstColumnSpanned(true);
    foreach(VisualFactory *factory, Visual::factories())
    {
        new MusicPluginItem(item, factory, Visual::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    item = new QTreeWidgetItem(m_ui->treeWidget, QStringList() << tr("Output"));
    item->setFirstColumnSpanned(true);
    foreach(OutputFactory *factory, Output::factories())
    {
        new MusicPluginItem(item, factory, Output::file(factory));
    }
    m_ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);

    m_ui->treeWidget->blockSignals(false);
    m_ui->treeWidget->resizeColumnToContents(0);
    m_ui->treeWidget->resizeColumnToContents(1);
}
