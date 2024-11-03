#include "DataLocalizer.h"

DataLocalizer::DataLocalizer(const std::string &input, const std::string &output):
    AbstractParser(input),
    AbstractSerializer(output)
{
}

void DataLocalizer::parse()
{
    std::vector<std::pair<std::string, std::string>> childrenPaths;

    std::filesystem::recursive_directory_iterator dirIt(m_input + "/" + DOFUS_DATA_PATH);
    std::filesystem::recursive_directory_iterator end;

    while (dirIt != end) {
        if (dirIt->is_regular_file()) {
            std::pair<std::string, std::string> pair;
            pair.first = dirIt->path().string();
            pair.second = m_output + "/" + DATA_PATH + dirIt->path().parent_path().string().substr(m_input.length() + std::string(DOFUS_DATA_PATH).length() + 1) + "/";

            Splitter quick(pair.first);
            quick.parse();

            bool isDataCenter = false;

            for (const InheritedClass &inherited : quick.getClassInfos().inheritedClasses) {
                if (inherited.name == "IDataCenter") {
                    isDataCenter = true;
                    break;
                }
            }

            if (isDataCenter) {
                Translator::addTranslated(quick.getClassInfos().name, quick.getClassInfos().name + "Data");
                childrenPaths.push_back(pair);
            } else {
                DataTranslator u(pair.first, pair.second);
                u.parse();
                m_uselessChildren<<u;
            }
        }
        ++dirIt;
    }

    for (size_t i = 0; i < childrenPaths.size(); i++) {
        m_children<<DataTranslator(childrenPaths[i].first, childrenPaths[i].second);
        m_children.last().parse();
    }
}

void DataLocalizer::serialize()
{
    std::unordered_map<std::string, int> toLoad;
    for(int i = 0; i < m_children.size(); i++)
        toLoad[m_children[i].getDofusName().toStdString()] = i;

    for(const auto &pair : toLoad)
        loadChild(pair.first, toLoad);
}

void DataLocalizer::write()
{
    for(int i = 0; i < m_children.size(); i++)
        m_children[i].write();
}

const QList<DataTranslator> &DataLocalizer::getChildren() const
{
    return m_children;
}

const QList<DataTranslator> &DataLocalizer::getUselessChildren() const
{
    return m_uselessChildren;
}

void DataLocalizer::loadChild(std::string childName, std::unordered_map<std::string, int> &toLoad, std::string previous, bool ignorePrevious)
{
    for(const QString &missingInclude : m_children[toLoad[childName]].getMissingIncludes())
    {
        std::string path;

        for(int i = 0; i < m_children.size(); i++)
        {
            if(m_children[i].getDofusName() == missingInclude)
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
            //            if(Translator::getLinkType(childName) == POINTER)
            //                m_children[toLoad[childName]].addAnticipatedDeclaration(missingInclude, path);

            //            else
            m_children[toLoad[childName]].addManualInclude(path);

            loadChild(missingInclude.toStdString(), toLoad, childName);
            toLoad.erase(missingInclude.toStdString());
        }

        else if(!ignorePrevious)
        {
            m_children[toLoad[childName]].addAnticipatedDeclaration(m_children[toLoad[missingInclude.toStdString()]].getName().toStdString(), path);
            loadChild(missingInclude.toStdString(), toLoad, childName, true);
        }
    }

    m_children[toLoad[childName]].serialize();
}

