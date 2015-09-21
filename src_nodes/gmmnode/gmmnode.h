
#ifndef GMMNODE_H
#define GMMNODE_H

#include "node/node.h"
#include "node/nodeconfig.h"
#include "rulesetdata/ruletypes.h"
#include "rulesetdata/consequent.h"
#include "rulesetdata/rule.h"
#include "rulesetdata/rulesetdata.h"
#include "tabledata/tabledata.h"
#include <QObject>
#include <QString>


class CGmmNode: public CNode
{
  Q_OBJECT

  typedef QMap<Antecedent, CRule> Ruleset;

  private:
    // Data Structures
    QSharedPointer<CRulesetData> m_ruleset;
    QSharedPointer<const CTableData> gmm_train_table;
    QSharedPointer<const CTableData> gmm_test_table;
    //QList<QSharedPointer<QVariant>> trainingData;

  public:
    // Constructor
    explicit CGmmNode(const CNodeConfig &config, QObject *parent = 0);
    // Set the configuration template for this Node.
    static void configure(CNodeConfig &config);

  protected:
    // Function called when the simulation is started.
    virtual bool start();
    // Receive data sent by other nodes connected to this node.
    virtual bool data(QString gate_name, const CConstDataPointer &data);
    // Trains the data
    void trainData(const QSharedPointer<const CTableData> &table);
    // Test the data
    void testData(const QSharedPointer<const CTableData> &table);
    //Gaussian Mixture model to train the data
    void gmmTrain(const QList<qint32> &training_data);
   // virtual float *vector(int n0,int n);
    float Normi(int x, float mu, float sd);
    float Normr(float x, float mu, float sd);
    inline qint32 rnd() const;
    // Write rules to a file.
   // void dumpRules(const QList<QString> &header,
     //              const QString &filename);
};

qint32 CGmmNode::rnd() const
{
    // 30-31 bit random number
    return (qrand() + (qrand() << 15)) & 0x7fffffff;
}

#endif // GMMNODE_H
