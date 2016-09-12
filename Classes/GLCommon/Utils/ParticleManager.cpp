#include "ParticleManager.h"

ParticleManager* ParticleManager::m_pInstance=NULL;
//ParticleManager::CGarbo ParticleManager::m_garbo;



ParticleManager* ParticleManager::getInstance()
{
	if ( m_pInstance == nullptr )
		m_pInstance = new ParticleManager();

	return m_pInstance;
}


void ParticleManager::destroyInstance()
{
	CC_SAFE_DELETE(m_pInstance);
}



ParticleManager::ParticleManager()
{
    m_plistMap.clear();
}


void ParticleManager::addPlistData(std::string strPlist,std::string strName)
{
    auto plistData=FileUtils::getInstance()->getValueMapFromFile(strPlist);
    std::map<std::string, ValueMap>::iterator it = m_plistMap.begin();
    m_plistMap.insert(it,std::pair<std::string, ValueMap>(strName,plistData));
}


ValueMap ParticleManager::getPlistData(std::string strplist)
{
    auto plistData = m_plistMap.find(strplist)->second;
    return plistData;
}
