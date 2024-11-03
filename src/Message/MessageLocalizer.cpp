#include "MessageLocalizer.h"

MessageLocalizer::MessageLocalizer(const std::string &input, const std::string &output):
    AbstractParser(input),
    AbstractSerializer(output)
{
}

MessageLocalizer::~MessageLocalizer()
{

}

void MessageLocalizer::parse()
{
    std::vector<std::pair<std::string, std::string>> childrenPaths;

    std::filesystem::recursive_directory_iterator dirIt(m_input + "/" + DOFUS_MESSAGE_PATH);
    std::filesystem::recursive_directory_iterator end;

    while (dirIt != end)
    {
        if (dirIt->is_regular_file())
        {
            std::pair<std::string, std::string> pair;
            pair.first = dirIt->path().string();
            pair.second = m_output+"/"+MESSAGE_PATH+dirIt->path().parent_path().string().substr(m_input.size() + 1 + std::string(DOFUS_MESSAGE_PATH).size());
            childrenPaths.emplace_back(pair);

            Splitter quick(pair.first);
            quick.parse();

            Translator::addTranslated(quick.getClassInfos().name);
        }

        ++dirIt;
    }

    for(int i = 0; i < childrenPaths.size(); i++)
    {
        m_children<<MessageTranslator(childrenPaths[i].first, childrenPaths[i].second);
        m_children.last().parse();
    }
}

void MessageLocalizer::serialize()
{
    std::unordered_map<std::string, int> toLoad;
    for(int i = 0; i < m_children.size(); i++)
        toLoad[m_children[i].getName().toStdString()] = i;

    for(const auto &pair : toLoad)
        loadChild(pair.first, toLoad);
}

void MessageLocalizer::write()
{
    for(int i = 0; i < m_children.size(); i++)
        m_children[i].write();
}

const QList<MessageTranslator> &MessageLocalizer::getChildren() const
{
    return m_children;
}

void MessageLocalizer::loadChild(std::string childName, std::unordered_map<std::string, int> &toLoad, std::string previous, bool ignorePrevious)
{
    foreach(const QString &missingInclude, m_children[toLoad[childName]].getMissingIncludes())
    {
        std::string path;

        for(int i = 0; i < m_children.size(); i++)
        {
            if(m_children[i].getName() == missingInclude)
            {
                path = m_children[i].getOutput();
                size_t pos = path.find(m_output); // Trouvez l'index de m_output
                if (pos != std::string::npos) {
                    path.erase(pos, m_output.length()); // Supprimez m_output de path
                }
                path.erase(0,1);
                path += m_children[i].getName().toStdString()+".h";
            }
        }

        if(missingInclude != previous)
        {
            if(Translator::getLinkType(QString::fromStdString(childName)) == POINTER)
                m_children[toLoad[childName]].addAnticipatedDeclaration(missingInclude.toStdString(), path);

            else
                m_children[toLoad[childName]].addManualInclude(path);

            loadChild(missingInclude.toStdString(), toLoad, childName);
            toLoad.erase(missingInclude.toStdString());
        }

        else if(!ignorePrevious)
        {
            m_children[toLoad[childName]].addAnticipatedDeclaration(missingInclude.toStdString(), path);
            loadChild(missingInclude.toStdString(), toLoad, childName, true);
        }
    }

    m_children[toLoad[childName]].serialize();
}
