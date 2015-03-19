#include "pythonnode.h"
#include "data/datafactory.h"
#include "data/messagedata.h"
#include "PythonQt.h"
#include <QDebug>
#include <QFile>


//------------------------------------------------------------------------------
// Constructor and Destructor

CPythonNode::CPythonNode(const CNodeConfig &config, QObject *parent/* = 0*/)
    : CNode(config, parent)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CPythonNode::configure(CNodeConfig &config)
{
    config.setDescription("Run Python scripts as nodes.");

    // Add parameters
    config.addFilename("script", "Python Script", "Python script to execute.");

    // Add the gates.
    //config.addInput("in", "misc");
    config.addOutput("out", "misc");
}


//------------------------------------------------------------------------------
// Protected Functions

bool CPythonNode::start()
{
    qDebug() << "Start called.";

    PythonQt::init(PythonQt::RedirectStdOut);
    QObject::connect(PythonQt::self(),
        &PythonQt::pythonStdOut,
        [](const QString &s) {
            qDebug() << s;
        });

    // Check if the script exists.
    QString script_name = getConfig().getParameter("script")->value.toString();
    QFile script_file(script_name);
    if(!script_file.exists()) {
        qWarning() << "Python script does not exist:"
                   << script_name;
        return false;
    }

    PythonQtObjectPtr python_obj =
            PythonQt::self()->createModuleFromFile("script", script_name);

    return true;
}

void CPythonNode::data(QString gate_name, const CConstDataPointer &data)
{
    Q_UNUSED(gate_name);
    qDebug() << "Data received.";

    // Process framework messages.
    if(data->getType() == "message") {
        auto pmsg = data.staticCast<const CMessageData>();
        QString msg = pmsg->getMessage();
        qDebug() << "Received message:" << msg;
        if(msg == "error") {
            commitError("out", "Could not get tcp file data.");
            return;
        }
    }
}

