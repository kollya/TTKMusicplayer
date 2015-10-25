#include "musicsongslistplaywidget.h"
#include "musicfileinformation.h"
#include "musicsongstoolitemrenamedwidget.h"
#include "musicuiobject.h"
#include "musicconnectionpool.h"

MusicSongsEnterPlayWidget::MusicSongsEnterPlayWidget(int index, QWidget *parent)
    : QWidget(parent), m_currentPlayIndex(index)
{
    QPalette pal = palette();
    pal.setBrush(QPalette::Base,QBrush(QColor(0, 0, 0, 100)));
    setPalette(pal);
    setAutoFillBackground(true);
}

void MusicSongsEnterPlayWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    emit enterChanged(m_currentPlayIndex, -1);
}


MusicSongsListPlayWidget::MusicSongsListPlayWidget(int index, QWidget *parent)
    : MusicSongsEnterPlayWidget(index, parent), m_renameLine(NULL)
{
    m_totalTime = "/00:00";

    m_artPicture = new QLabel(this);
    m_artPicture->setFixedSize(60, 60);
    m_artPicture->setAttribute(Qt::WA_TranslucentBackground);
    m_artPicture->setGeometry(0, 0, 60, 60);

    m_songName = new QLabel(this);
    m_songName->setFixedSize(202, 25);
    m_songName->setAttribute(Qt::WA_TranslucentBackground);
    m_songName->setStyleSheet(MusicUIObject::MCustomStyle11);
    m_songName->setGeometry(65, 5, 182, 25);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setFixedSize(100, 20);
    m_timeLabel->setAttribute(Qt::WA_TranslucentBackground);
    m_timeLabel->setStyleSheet(MusicUIObject::MCustomStyle11);
    m_timeLabel->setGeometry(65, 37, 100, 20);

    m_columnOne = new MusicSongsEnterPlayWidget(index, this);
    m_columnThree = new MusicSongsEnterPlayWidget(index, this);

    m_loveButton = new QPushButton(this);
    m_loveButton->setGeometry(204, 35, 23, 23);
    m_loveButton->setStyleSheet( MusicUIObject::MPushButtonStyle13 );
    m_loveButton->setIcon(QIcon(":/image/bestlove"));
    m_loveButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_loveButton->setToolTip(tr("bestlove"));

    m_deleteButton = new QPushButton(this);
    m_deleteButton->setGeometry(227, 35, 23, 23);
    m_deleteButton->setStyleSheet( MusicUIObject::MPushButtonStyle13 );
    m_deleteButton->setIcon(QIcon(":/image/musicdelete"));
    m_deleteButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_deleteButton->setToolTip(tr("deleteMusic"));

    m_showMVButton = new QPushButton(this);
    m_showMVButton->setGeometry(250, 35, 23, 23);
    m_showMVButton->setStyleSheet( MusicUIObject::MPushButtonStyle13 );
    m_showMVButton->setIcon(QIcon(":/share/showMV"));
    m_showMVButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_showMVButton->setToolTip(tr("showMV"));

    m_songShareButton = new QPushButton(this);
    m_songShareButton->setGeometry(250, 7, 23, 23);
    m_songShareButton->setStyleSheet( MusicUIObject::MPushButtonStyle13 );
    m_songShareButton->setIcon(QIcon(":/image/songShare"));
    m_songShareButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_songShareButton->setToolTip(tr("songShare"));

    connect(m_loveButton, SIGNAL(clicked()), parent, SLOT(addMusicSongToLovestListAt()));
    connect(m_deleteButton, SIGNAL(clicked()), parent, SLOT(setDeleteItemAt()));
    connect(this, SIGNAL(renameFinished(QString)), parent, SLOT(setItemRenameFinished(QString)));
    connect(this, SIGNAL(enterChanged(int,int)), parent, SLOT(listCellEntered(int,int)));
    connect(m_columnOne, SIGNAL(enterChanged(int,int)), parent, SLOT(listCellEntered(int,int)));
    connect(m_columnThree, SIGNAL(enterChanged(int,int)), parent, SLOT(listCellEntered(int,int)));
    connect(m_showMVButton, SIGNAL(clicked()), SLOT(showMVButtonClicked()));

    M_Connection->setValue("MusicSongsListPlayWidget", this);
    M_Connection->connect("MusicSongsListPlayWidget", "MusicRightAreaWidget");
}

MusicSongsListPlayWidget::~MusicSongsListPlayWidget()
{
    M_Connection->disConnect("MusicSongsListPlayWidget");
    delete m_renameLine;
    delete m_artPicture;
    delete m_songName;
    delete m_timeLabel;
    delete m_columnOne;
    delete m_columnThree;
    delete m_loveButton;
    delete m_deleteButton;
    delete m_showMVButton;
    delete m_songShareButton;
}

void MusicSongsListPlayWidget::getWidget(QWidget *&one, QWidget *&two) const
{
    one = m_columnOne;
    two = m_columnThree;
}

void MusicSongsListPlayWidget::insertTimerLabel(const QString &t) const
{
    m_timeLabel->setText(t + m_totalTime);
}

void MusicSongsListPlayWidget::updateArtPicture() const
{
    showArtPicture(m_songName->text().split('-').front().trimmed());
}

bool MusicSongsListPlayWidget::showArtPicture(const QString &name) const
{
    QPixmap originPath(QString(MusicObject::getAppDir() + ART_DOWNLOAD + name + SKN_FILE));
    if(!originPath.isNull())
    {
        m_artPicture->setPixmap(originPath.scaled(60, 60));
        return true;
    }
    return false;
}

void MusicSongsListPlayWidget::setParameter(const QString &name,const QString &path)
{
    MusicFileInformation info;
    if(info.readFile(path))
    {
        m_totalTime = "/" + info.getLengthString();
    }
    m_songName->setText(QFontMetrics(font()).elidedText(name, Qt::ElideRight, 180));
    m_songName->setToolTip(name);
    m_timeLabel->setText("00:00" + m_totalTime);

    if(!showArtPicture(name.split('-').front().trimmed()) &&
       !showArtPicture(name.split('-').back().trimmed()) )
    {
        m_artPicture->setPixmap(QPixmap(":/share/defaultArt").scaled(60, 60));
    }
}

void MusicSongsListPlayWidget::setItemRename()
{
    m_renameLine = new MusicSongsToolItemRenamedWidget(0, m_songName->text(), this);
    connect(m_renameLine, SIGNAL(renameFinished(QString)), SLOT(setChangItemName(QString)));
    m_renameLine->setFixedSize(182, 25);
    m_renameLine->setGeometry(65, 5, 182, 25);
    m_renameLine->show();
}

void MusicSongsListPlayWidget::setChangItemName(const QString &name)
{
    m_songName->setText(name);
    m_songName->setToolTip(name);
    emit renameFinished(name);
    delete m_renameLine;
    m_renameLine = NULL;
}

void MusicSongsListPlayWidget::showMVButtonClicked()
{
    emit videoButtonClicked(m_songName->text());
}
