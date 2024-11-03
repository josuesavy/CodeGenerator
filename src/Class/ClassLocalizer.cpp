#include "ClassLocalizer.h"

ClassLocalizer::ClassLocalizer(const std::string &input, const std::string &output):
    AbstractParser(input),
    AbstractSerializer(output)
{

}

ClassLocalizer::~ClassLocalizer()
{
}

void ClassLocalizer::parse()
{
    std::filesystem::path inputPath = m_input+"/"+DOFUS_CLASS_PATH;
    std::vector<std::pair<std::string, std::string>> childrenPaths;

    std::filesystem::recursive_directory_iterator dirIt(inputPath), end;

    while (dirIt != end)
    {
        if (dirIt->is_regular_file())
        {
            std::pair<std::string, std::string> pair;
            pair.first = dirIt->path().string();
            pair.second = m_output + "/" + CLASS_PATH + dirIt->path().parent_path().string().substr(m_input.size() + 1 + std::string(DOFUS_CLASS_PATH).size());
            childrenPaths.emplace_back(pair);
            Splitter quick(pair.first);
            quick.parse();

            Translator::addTranslated(quick.getClassInfos().name,
                                      std::find(m_derivableChildren.begin(), m_derivableChildren.end(), quick.getClassInfos().name) != m_derivableChildren.end());
        }
        ++dirIt;
    }

    for(int i = 0; i < childrenPaths.size(); i++)
    {
        m_children<<ClassTranslator(childrenPaths[i].first, childrenPaths[i].second);
        m_children.last().parse();
    }
}

void ClassLocalizer::serialize()
{
    std::unordered_map<std::string, int> toLoad;

    for(int i = 0; i < m_children.size(); i++)
        toLoad[m_children[i].getName().toStdString()] = i;

    for(std::string x : m_derivableChildren)
        orderChild(x, toLoad);

    for(const auto &pair : toLoad)
        orderChild(pair.first, toLoad);

    for(int i = 0; i < m_orderedChildren.size(); i++)
        toLoad[m_orderedChildren[i]->getName().toStdString()] = i;

    for(ClassTranslator *child : m_orderedChildren)
    {
        if(Translator::getLinkType(child->getName()) != SHARED_POINTER && hasPointers(child->getName().toStdString(), toLoad))
            Translator::addTranslated(child->getName(), true);
    }

    for(ClassTranslator *child : m_orderedChildren)
        child->serialize();
}

void ClassLocalizer::write()
{
    for(int i = 0; i < m_children.size(); i++)
        m_children[i].write();
}

void ClassLocalizer::setDerivableChildren(const QStringList &derivableChildren)
{
    std::vector<std::string> stdVector;
    stdVector.reserve(derivableChildren.size());

    for (const QString& qstr : derivableChildren) {
        stdVector.push_back(qstr.toStdString());
    }

    m_derivableChildren = stdVector;
}

const QList<ClassTranslator> &ClassLocalizer::getChildren() const
{
    return m_children;
}

void ClassLocalizer::orderChild(std::string childName, std::unordered_map<std::string, int> &toLoad, std::string previous, bool ignorePrevious)
{
    for (const QString &missingInclude : m_children[toLoad[childName]].getMissingIncludes())
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
            //            if(Translator::getLinkType(childName) == POINTER && m_children[toLoad[childName]].getrequiredIncludes())
            //                m_children[toLoad[childName]].addAnticipatedDeclaration(missingInclude, path);

            //            else
            m_children[toLoad[childName]].addManualInclude(path);

            orderChild(missingInclude.toStdString(), toLoad, childName);
            toLoad.erase(missingInclude.toStdString());
        }

        else if(!ignorePrevious)
        {
            Translator::addTranslated(missingInclude, true);
            m_children[toLoad[childName]].addAnticipatedDeclaration(missingInclude.toStdString(), path);
            orderChild(missingInclude.toStdString(), toLoad, childName, true);
        }
    }

    m_orderedChildren.push_back(&m_children[toLoad[childName]]);
}

bool ClassLocalizer::hasPointers(std::string childName, std::unordered_map<std::string, int> &toLoad)
{
    for (const ClassVariable &classVariable : m_orderedChildren[toLoad[childName]]->getClassVariables())
    {
        if(classVariable.variable.isContainer && Translator::isTranslated(classVariable.variable.containerShell.type) && Translator::getLinkType(classVariable.variable.containerShell.type) == SHARED_POINTER)
            return true;

        if(Translator::isTranslated(classVariable.variable.type) && Translator::getLinkType(classVariable.variable.type) == SHARED_POINTER)
            return true;
    }

    for (const InheritedClass &inheritedClass : m_orderedChildren[toLoad[childName]]->getInheritedClasses())
    {
        if(Translator::isTranslated(inheritedClass.name))
        {
            if (toLoad.find(inheritedClass.name.toStdString()) != toLoad.end() && hasPointers(inheritedClass.name.toStdString(), toLoad))
                return true;

            else
                toLoad.erase(inheritedClass.name.toStdString());
        }
    }

    return false;
}
