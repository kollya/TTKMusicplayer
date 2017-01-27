#ifndef MUSICSONGCHECKTOOLSUNIT_H
#define MUSICSONGCHECKTOOLSUNIT_H

#include "musicsong.h"
#include "musicobject.h"

typedef struct MUSIC_TOOLSET_EXPORT SongCheckToolsRename
{
    QString m_locaName;
    QString m_RecommendName;

    SongCheckToolsRename(const QString &locaName, const QString &recommendName)
    {
        m_locaName = locaName;
        m_RecommendName = recommendName;
    }
}SongCheckToolsRename;
TTK_DECLARE_LISTS(SongCheckToolsRename)


typedef struct MUSIC_TOOLSET_EXPORT SongCheckToolsDuplicate
{
    MusicSong m_song;
    QString m_bitRate;

    SongCheckToolsDuplicate(const MusicSong &song, const QString &bitRate)
    {
        m_song = song;
        m_bitRate = bitRate;
    }
}SongCheckToolsDuplicate;
TTK_DECLARE_LISTS(SongCheckToolsDuplicate)


typedef struct MUSIC_TOOLSET_EXPORT SongCheckToolsQuality
{
    MusicSong m_song;
    QString m_bitRate;
    int m_quality;

    SongCheckToolsQuality(const MusicSong &song, const QString &bitRate, int quality)
    {
        m_song = song;
        m_bitRate = bitRate;
        m_quality = quality;
    }
}SongCheckToolsQuality;
TTK_DECLARE_LISTS(SongCheckToolsQuality)

#endif // MUSICSONGCHECKTOOLSUNIT_H
