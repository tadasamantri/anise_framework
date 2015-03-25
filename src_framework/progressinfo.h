#ifndef CPROGRESSINFO_H
#define CPROGRESSINFO_H

#include <QVariant>

class CProgressInfo
{
  public:
    enum class ESource {null, node, framework};
    enum class EState {null, init, processing};
    enum class EMsg {null, start, stop, percentage, error, warning};

    CProgressInfo();

    // Convert this progress information to a string that we can show to the user.
    QString toJsonString();
    // Print the json representation of this progress if progress reporting
    // ... is enabled.
    void printProgress();

    ESource src() const;
    QString srcString() const;
    void setSrc(ESource src);

    EState state() const;
    QString stateString() const;
    void setState(EState state);

    EMsg msg() const;
    QString msgString() const;
    void setMsg(EMsg msg);

    QVariant info() const;
    void setInfo(QString info);
    void setInfo(qint32 info);

    QString name() const;
    void setName(QString name);

private:
    ESource m_src;
    EState m_state;
    EMsg m_msg;
    QString m_name;
    QVariant m_info;
};

#endif // CPROGRESSINFO_H
