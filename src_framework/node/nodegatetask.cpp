#include "nodegatetask.h"
#include "../settings.h"
#include "../progressinfo.h"


//------------------------------------------------------------------------------
// Constructor and Destructor

CNodeGateTask::CNodeGateTask(CNode &node, QString gate_name,
    const CConstDataPointer &data, QObject *parent /* = 0 */)
    : QObject(parent)
    , m_node(node)
    , m_gate_name(gate_name)
    , m_data(data)
{

}


//------------------------------------------------------------------------------
// Public Functions

void CNodeGateTask::run()
{
    // Report that we are starting, if appropriate.
    CProgressInfo progress;
    progress.setSrc(CProgressInfo::ESource::node);
    progress.setState(CProgressInfo::EState::processing);
    progress.setName(m_node.getConfig().getName());

    if(CSettings::progress()) {
        progress.setMsg(CProgressInfo::EMsg::start);
        progress.printProgress();
    }

    // Enable the usage of the commit functions only while the data function
    // ... is called.
    m_node.m_allow_commit = true;
    // Perform the actual processing of the data.
    m_node.data(m_gate_name, m_data);
    // Dissalow the commit functions outside of the nodes' data function.
    m_node.m_allow_commit = false;

    // Report that we are finished processing, if apropriate.
    if(CSettings::progress()) {
        progress.setMsg(CProgressInfo::EMsg::stop);
        progress.printProgress();
    }

    // Tell whoever is interested that the task has been finished.
    emit taskFinished();
}

//------------------------------------------------------------------------------
// Private Functions
