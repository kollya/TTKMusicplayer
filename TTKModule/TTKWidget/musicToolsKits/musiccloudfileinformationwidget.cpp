#include "musiccloudfileinformationwidget.h"
#include "ui_musiccloudfileinformationwidget.h"
#include "musicuiobject.h"
#include "musicnumberutils.h"
#///Oss import
#include "qoss/qossdataitem.h"

MusicCloudFileInformationWidget::MusicCloudFileInformationWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      m_ui(new Ui::MusicCloudFileInformationWidget)
{
    m_ui->setupUi(this);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));
}

MusicCloudFileInformationWidget::~MusicCloudFileInformationWidget()
{
    delete m_ui;
}

void MusicCloudFileInformationWidget::setFileInformation(QOSSDataItem *data)
{
    m_ui->filePathEdit->setText(data->m_name);
    m_ui->fileFormatEdit->setText(data->m_mimeType);
    m_ui->fileSizeEdit->setText(MusicUtils::Number::size2Label(data->m_size));
    m_ui->fileKeyEdit->setText(data->m_hash);
    m_ui->fileUpdateTimeEdit->setText(data->m_putTime);
}

int MusicCloudFileInformationWidget::exec()
{
    setBackgroundPixmap(m_ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
